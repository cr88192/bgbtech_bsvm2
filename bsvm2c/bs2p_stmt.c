/*
Copyright (C) 2015-2016 by Brendan G Bohannon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/


int BS2P_ParseError(BS2CC_CompileContext *ctx, int errn)
{
	char *lfn;
	int lln;
	int i;

	BS2P_GetCurSourceLine(ctx, &lfn, &lln);
	ctx->srcfn=lfn;
	ctx->srcln=lln;
	
	i=BS2C_CompileError(ctx, errn);
	return(i);
}

void BS2P_WarnExpectSemi(BS2CC_CompileContext *ctx)
{
	BS2P_ParseError(ctx, BS2CC_ERRN_NOSEMICOLON);
}

void BS2P_ErrSyntaxError(BS2CC_CompileContext *ctx)
{
	BS2P_ParseError(ctx, BS2CC_ERRN_ERRSYNTAX);
}

int BS2P_LangIsBS2(BS2CC_CompileContext *ctx)
{
	if(!ctx->srclang)
		return(1);
	if(ctx->srclang==BS2CC_SRCLANG_BS2)
		return(1);
	return(0);
}

int BS2P_LangIsC(BS2CC_CompileContext *ctx)
{
	if(ctx->srclang==BS2CC_SRCLANG_C)
		return(1);
	if(ctx->srclang==BS2CC_SRCLANG_CPP)
		return(1);
	return(0);
}

dtVal BS2P_ParseInnerStatement(BS2CC_CompileContext *ctx)
{
	dtVal n0, n1, n2;
	char *t0, *t1, *t2;
	int i, j, k;

	t0=BS2P_PeekToken(ctx, 0);

	if(t0 && (*t0=='I'))
	{
		if(!strcmp(t0, "Ibreak"))
		{
			BS2P_NextToken(ctx);
			i=1;
			
			t0=BS2P_PeekToken(ctx, 0);
			if(*t0=='|')
			{
				i=atoi(t0+1);
				BS2P_NextToken(ctx);
			}else if(strcmp(t0, "X;"))
			{
				n1=BS2P_ParseExpr(ctx);
				i=dtvUnwrapLong(n1);
			}
			
			n0=BS2P_ParseWrapSimpleTagInt(ctx, "break", i);
//			t0=BS2P_PeekToken(ctx, 0);
			return(n0);
		}

		if(!strcmp(t0, "Icontinue"))
		{
			BS2P_NextToken(ctx);
			i=0;
			
			t0=BS2P_PeekToken(ctx, 0);
			if(*t0=='|')
			{
				i=atoi(t0+1);
				BS2P_NextToken(ctx);
			}else if(strcmp(t0, "X;"))
			{
				n1=BS2P_ParseExpr(ctx);
				i=dtvUnwrapLong(n1);
			}
			
			n0=BS2P_ParseWrapSimpleTagInt(ctx, "continue", i);
//			t0=BS2P_PeekToken(ctx, 0);
			return(n0);
		}

		if(!strcmp(t0, "Idelete"))
		{
			BS2P_NextToken(ctx);
			t0=BS2P_PeekToken(ctx, 0);
//			n1=BS2P_ParseExpr(ctx);
			n1=BS2P_ParseExprList(ctx);
			n0=BS2P_ParseWrapSimpleTagVal(ctx, "delete", n1);
			return(n0);
		}

		if(!strcmp(t0, "Igoto"))
		{
			BS2P_NextToken(ctx);
			n1=BS2P_ParseExpr(ctx);
			n0=BS2P_ParseWrapSimpleTagVal(ctx, "goto", n1);
			return(n0);
		}

		if(!strcmp(t0, "Ireturn"))
		{
			BS2P_NextToken(ctx);
			t0=BS2P_PeekToken(ctx, 0);
			if(strcmp(t0, "X;"))
				{ n1=BS2P_ParseExpr(ctx); }
			else
				{ n1=DTV_NULL; }
			n0=BS2P_ParseWrapSimpleTagVal(ctx, "return", n1);
			return(n0);
		}

		if(!strcmp(t0, "Ithrow"))
		{
			BS2P_NextToken(ctx);
			n1=BS2P_ParseExpr(ctx);
			n0=BS2P_ParseWrapSimpleTagVal(ctx, "throw", n1);
			return(n0);
		}
	}
	
	return(DTV_NULL);
	
//	n0=BS2P_ParseExpr(ctx);
//	return(n0);
}

dtVal BS2P_ParseStatementBlock(BS2CC_CompileContext *ctx, int flag)
{
	dtVal n0, n1, n2, n3;
	char *t0, *t1, *t2, *lfn;
	dtVal stmt[4096];
	int nstmt, lln;

	nstmt=0;
	t0=BS2P_PeekToken(ctx, 0);
	while(t0 && *t0)
	{
		if(!strcmp(t0, "X}"))
			break;

		BS2P_GetCurSourceLine(ctx, &lfn, &lln);

		n0=BS2P_ParseBlockStatement(ctx);

		if(BGBDT_MapObj_IsObjectP(n0))
		{
			BS2P_SetAstNodeAttrStr(n0, "fn", lfn);
			BS2P_SetAstNodeAttrI(n0, "ln", lln);
		}
		
		if(nstmt>=4095)
		{
			BSVM2_DBGTRAP
			break;
		}
		
		stmt[nstmt++]=n0;
		t0=BS2P_PeekToken(ctx, 0);
	}
	
	if(!nstmt)
	{
		n0=BS2P_ParseWrapSimpleTag(ctx, "empty_block");
		return(n0);
	}
	
	if(nstmt==1)
	{
		return(stmt[0]);
	}
	
	n1=BS2P_NewAstWrapArray(ctx, stmt, nstmt);
	n0=BS2P_ParseWrapSimpleTagVal(ctx, "block", n1);
	return(n0);
}

dtVal BS2P_ParseBlockStatement(BS2CC_CompileContext *ctx)
{
	return(BS2P_ParseBlockStatementI(ctx, 0));
}

dtVal BS2P_ParseBlockStatementTail(BS2CC_CompileContext *ctx)
{
	return(BS2P_ParseBlockStatementI(ctx, 1));
}

dtVal BS2P_ParseBlockStatementTail2(BS2CC_CompileContext *ctx)
{
	return(BS2P_ParseBlockStatementI(ctx, 3));
}

dtVal BS2P_ParseBlockStatementI(BS2CC_CompileContext *ctx, int flag)
{
	dtVal n0, n1, n2, n3;
	char *t0, *t1, *t2;
	int i, j, k;

	t0=BS2P_PeekToken(ctx, 0);
	t1=BS2P_PeekToken(ctx, 1);

	if(t0 && !strcmp(t0, "X{"))
	{
		BS2P_NextToken(ctx);
		n0=BS2P_ParseStatementBlock(ctx, flag&(~2));
		BS2P_ParseExpectToken(ctx, "X}");
		return(n0);
	}

	if(t0 && (*t0=='I'))
	{
		if(!strcmp(t0, "Icase"))
		{
			BS2P_NextToken(ctx);
			n1=BS2P_ParseExpr(ctx);
			BS2P_ParseExpectToken(ctx, "X:");

			n0=BS2P_ParseWrapSimpleTagVal(ctx, "case", n1);
			return(n0);
		}

		if(!strcmp(t0, "Idefault"))
		{
			BS2P_NextToken(ctx);
//			n1=BS2P_ParseExpr(ctx);
			BS2P_ParseExpectToken(ctx, "X:");

			n0=BS2P_ParseWrapSimpleTag(ctx, "case_default");
			return(n0);
		}

		if(!strcmp(t0, "Ifor"))
		{
			BS2P_NextToken(ctx);
			
			BS2P_ParseExpectToken(ctx, "X(");
			n0=BS2P_ParseOptExpr(ctx);
			BS2P_ParseExpectToken(ctx, "X;");
			n1=BS2P_ParseOptExpr(ctx);
			BS2P_ParseExpectToken(ctx, "X;");
			n2=BS2P_ParseOptExpr(ctx);
			BS2P_ParseExpectToken(ctx, "X)");
			n3=BS2P_ParseBlockStatement(ctx);
			
			n0=BS2P_ParseWrapTagFor(ctx, "for", n0, n1, n2, n3);
			return(n0);
		}

		if(!strcmp(t0, "Iif"))
		{
			BS2P_NextToken(ctx);
			
			BS2P_ParseExpectToken(ctx, "X(");
			n0=BS2P_ParseExprAssignOp(ctx);
			BS2P_ParseExpectToken(ctx, "X)");
			n1=BS2P_ParseBlockStatement(ctx);

			if(BS2P_ParseExpectOptToken(ctx, "Ielse"))
			{
				n2=BS2P_ParseBlockStatement(ctx);
				n0=BS2P_ParseWrapTagIfElse(ctx, "ifelse", n0, n1, n2);
			}else
			{
				n0=BS2P_ParseWrapTagIf(ctx, "if", n0, n1);
			}
			return(n0);
		}

		if(!strcmp(t0, "Iswitch"))
		{
			BS2P_NextToken(ctx);
			
			BS2P_ParseExpectToken(ctx, "X(");
			n0=BS2P_ParseExprAssignOp(ctx);
			BS2P_ParseExpectToken(ctx, "X)");
			n1=BS2P_ParseBlockStatement(ctx);
			n0=BS2P_ParseWrapTagIf(ctx, "switch", n0, n1);
			return(n0);
		}

		if(!strcmp(t0, "Iwhile"))
		{
			BS2P_NextToken(ctx);
			
			BS2P_ParseExpectToken(ctx, "X(");
			n0=BS2P_ParseExprAssignOp(ctx);
			BS2P_ParseExpectToken(ctx, "X)");
			n1=BS2P_ParseBlockStatement(ctx);
			n0=BS2P_ParseWrapTagIf(ctx, "while", n0, n1);
			return(n0);
		}

		if(!strcmp(t0, "Ido"))
		{
			BS2P_NextToken(ctx);

			n1=BS2P_ParseBlockStatement(ctx);

			BS2P_ParseExpectToken(ctx, "Iwhile");
			
			BS2P_ParseExpectToken(ctx, "X(");
			n0=BS2P_ParseExprAssignOp(ctx);
			BS2P_ParseExpectToken(ctx, "X)");
			n0=BS2P_ParseWrapTagIf(ctx, "do_while", n0, n1);
			BS2P_ParseExpectOptToken(ctx, "X;");
			return(n0);
		}

		if(!strcmp(t0, "Itry"))
		{
			BS2P_NextToken(ctx);
			
//			BS2P_ParseExpectToken(ctx, "X(");
//			n0=BS2P_ParseExprAssignOp(ctx);
//			BS2P_ParseExpectToken(ctx, "X)");
			n1=BS2P_ParseBlockStatement(ctx);
//			n0=BS2P_ParseWrapTagIf(ctx, "while", n0, n1);
			n0=BS2P_ParseWrapTagBody(ctx, "try", n1);
//			n0=n1;
			
			while(BS2P_ParseExpectOptToken(ctx, "Icatch"))
			{
				BS2P_ParseExpectToken(ctx, "X(");
//				n1=BS2P_ParseExprAssignOp(ctx);
				n1=BS2P_TryParseArgDecl(ctx);
				BS2P_ParseExpectToken(ctx, "X)");
				
				n2=BS2P_ParseBlockStatement(ctx);
				n0=BS2P_ParseWrapTagCatch(ctx, "catch", n1, n0, n2);
//				n0=BS2P_ParseWrapTagIfElse(ctx, "ifelse", n0, n1, n2);
			}

			if(BS2P_ParseExpectOptToken(ctx, "Ifinally"))
			{
				n2=BS2P_ParseBlockStatement(ctx);
				n0=BS2P_ParseWrapBinTag(ctx, "finally", n0, n2);
			}
			
			return(n0);
		}

		if(!strcmp(t0, "Ilet_escape"))
		{
			BS2P_NextToken(ctx);

			BS2P_ParseExpectToken(ctx, "X(");
			n0=BS2P_TryParseArgDecl(ctx);
			if(!dtvTrueP(n0))
				n0=BS2P_ParseExprAssignOp(ctx);
			BS2P_ParseExpectToken(ctx, "X)");

			n1=BS2P_ParseBlockStatement(ctx);

			n0=BS2P_ParseWrapTagIf(ctx, "let_escape", n0, n1);
//			BS2P_ParseExpectOptToken(ctx, "X;");
			return(n0);
		}

		if(!strcmp(t1, "X:"))
		{
			BS2P_NextToken(ctx);
			BS2P_NextToken(ctx);

			n0=BS2P_ParseWrapSimpleCmdStr(ctx, "label", t0+1);
			return(n0);
		}
	}

	if(flag&2)
	{
		i=BS2P_GetCurPosition(ctx);
	
		n0=BS2P_ParseInnerStatement(ctx);
		if(dtvTrueP(n0))
		{
			BS2P_ParseExpectToken(ctx, "X;");
			return(n0);
		}

		n0=BS2P_ParseExpr(ctx);
		if(dtvTrueP(n0))
		{
			if(BS2P_ParseExpectOptToken(ctx, "X;"))
			{
				if((flag&1) &&
					!BS2P_CheckAstNodeTag(n0, "assign") &&
					!BS2P_CheckAstNodeTag(n0, "assignop"))
				{
					n0=BS2P_ParseWrapSimpleTagVal(ctx, "tail", n0);
				}
			}else
			{
				BS2P_WarnExpectSemi(ctx);
				BS2P_SetCurPosition(ctx, i);
				return(DTV_NULL);
			}
		}else
		{
			n0=DTV_NULL;
			if(!BS2P_ParseExpectOptToken(ctx, "X;"))
				BS2P_WarnExpectSemi(ctx);
		}
		return(n0);
	}

	n0=BS2P_TryParseDecl(ctx);
	if(dtvTrueP(n0))
		return(n0);

	n0=BS2P_ParseInnerStatement(ctx);
	if(dtvTrueP(n0))
	{
		if(!BS2P_ParseExpectOptToken(ctx, "X;"))
			BS2P_WarnExpectSemi(ctx);
		return(n0);
	}
	
//	n0=BS2P_ParseInnerStatement(ctx);
	n0=BS2P_ParseExpr(ctx);
	if(dtvTrueP(n0))
	{
		if(!BS2P_ParseExpectOptToken(ctx, "X;"))
		{
			t0=BS2P_PeekToken(ctx, 0);
			if(t0 && !strcmp(t0, "X}"))
				n0=BS2P_ParseWrapSimpleTagVal(ctx, "tail", n0);
		}else if((flag&1) &&
			!BS2P_CheckAstNodeTag(n0, "assign") &&
			!BS2P_CheckAstNodeTag(n0, "assignop"))
		{
			n0=BS2P_ParseWrapSimpleTagVal(ctx, "tail", n0);
		}
	}else
	{
//		BS2P_ParseExpectOptToken(ctx, "X;");
		if(!BS2P_ParseExpectOptToken(ctx, "X;"))
			BS2P_WarnExpectSemi(ctx);
	}

	return(n0);
}


dtVal BS2P_ParsePackageStatement(BS2CC_CompileContext *ctx)
{
	dtVal n0, n1, n2, n3;
	char *t0, *t1, *t2;
	int i, j, k;

	t0=BS2P_PeekToken(ctx, 0);
	t1=BS2P_PeekToken(ctx, 1);

	if(t0 && !strcmp(t0, "X{"))
	{
		BS2P_NextToken(ctx);
		n0=BS2P_ParsePackageStatementBlock(ctx);
		BS2P_ParseExpectToken(ctx, "X}");
		return(n0);
	}

#if 0
	if(t0 && (*t0=='I'))
	{
		if(!strcmp(t0, "Ipackage"))
		{
			BS2P_NextToken(ctx);
			i=0;
		}
	}
#endif

	n0=BS2P_TryParseDecl(ctx);
	if(dtvTrueP(n0))
		return(n0);

	return(DTV_NULL);
}

dtVal BS2P_ParsePackageStatementBlock(BS2CC_CompileContext *ctx)
{
	dtVal n0, n1, n2, n3;
	char *t0, *t1, *t2, *lfn;
	dtVal stmt[4096];
	int nstmt, lln;

	nstmt=0;
	t0=BS2P_PeekToken(ctx, 0);
	while(t0 && *t0)
	{
		if(!strcmp(t0, "X}"))
			break;
		BS2P_GetCurSourceLine(ctx, &lfn, &lln);
		n0=BS2P_ParsePackageStatement(ctx);
		
		if(dtvNullP(n0))
		{
			BS2P_ErrSyntaxError(ctx);
			return(DTV_NULL);
		}
		
		if(BGBDT_MapObj_IsObjectP(n0))
		{
			BS2P_SetAstNodeAttrStr(n0, "fn", lfn);
			BS2P_SetAstNodeAttrI(n0, "ln", lln);
		}
		
		if(nstmt>=4096)
		{
			BS2P_ErrSyntaxError(ctx);
			BSVM2_DBGTRAP
			return(DTV_NULL);
		}
		
		stmt[nstmt++]=n0;
		t0=BS2P_PeekToken(ctx, 0);
	}
	
	n1=BS2P_NewAstWrapArray(ctx, stmt, nstmt);
	n0=BS2P_ParseWrapSimpleTagVal(ctx, "psblock", n1);
	return(n0);
}


BS2VM_API dtVal BS2P_ParseBuffer(BS2CC_CompileContext *ctx,
	char *srcbuf, int szsrcbuf)
{
	dtVal n0;

	BS2P_LexBuffer(ctx, srcbuf, szsrcbuf);
	n0=BS2P_ParsePackageStatementBlock(ctx);
	BS2P_CleanupLexBuffer(ctx);
	
	return(n0);
}

BS2VM_API dtVal BS2P_ParseBufferExpression(BS2CC_CompileContext *ctx,
	char *srcbuf, int szsrcbuf)
{
	dtVal n0;

	BS2P_LexBuffer(ctx, srcbuf, szsrcbuf);
	n0=BS2P_ParseExpr(ctx);
	BS2P_CleanupLexBuffer(ctx);
	
	return(n0);
}

BS2VM_API BS2CC_CompileContext *BS2CC_AllocCompileContext()
{
	BS2CC_CompileContext *ctx;
	
	ctx=dtmAlloc("bs2cc_compilecontext_t",
		sizeof(BS2CC_CompileContext));
	ctx->gsseq=1;
	return(ctx);
}

BS2VM_API void BS2P_FreeCompileContext(BS2CC_CompileContext *ctx)
{
	BS2CC_VarInfo *vcur, *vnxt;
	BS2CC_CcFrame *fcur, *fnxt;

	vcur=ctx->lst_vari;
	while(vcur)
	{
		vnxt=vcur->alcnext;
		BS2C_CleanupFreeVarInfo(ctx, vcur);
		vcur=vnxt;
	}

	fcur=ctx->lst_ccfrm;
	while(fcur)
	{
		fnxt=fcur->alcnext;
		BS2C_CleanupFreeCcFrame(ctx, fcur);
		fcur=fnxt;
	}

	if(ctx->strtab)
	{
		bgbdt_mm_free(ctx->strtab);
		ctx->strtab=NULL;
	}

	dtmFree(ctx);
}