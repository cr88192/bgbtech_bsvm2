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


char *bs2p_decl_nontypekeyword[]={
"as",		"break",	"case",		"catch",
"class",	"continue",	"default",	"delete",
"do",		"finally",	"for",		"function",
"goto",		"if",		"import",	"is",
"new",		"package",	"return",	"sizeof",
"struct",	"switch",	"throw",	"try",
"var",		"while",
NULL};

int BS2P_DeclTypeRejectKeyword(BS2CC_CompileContext *ctx, char *str)
{
	int i;
	
	for(i=0; bs2p_decl_nontypekeyword[i]; i++)
		if(!strcmp(bs2p_decl_nontypekeyword[i], str))
			return(i+1);
	
	return(0);
}

dtVal BS2P_ParseModifierList(BS2CC_CompileContext *ctx)
{
	struct {
	char *name;
	int fl;
	}modfl[]={
		{"public",			BS2CC_TYFL_PUBLIC},
		{"private",			BS2CC_TYFL_PRIVATE},
		{"protected",		BS2CC_TYFL_PROTECTED},
		{"static",			BS2CC_TYFL_STATIC},
		{"final",			BS2CC_TYFL_FINAL},
		{"synchronized",	BS2CC_TYFL_SYNCHRONIZED},
		{"volatile",		BS2CC_TYFL_VOLATILE},
		{"transient",		BS2CC_TYFL_TRANSIENT},
		{"native",			BS2CC_TYFL_NATIVE},
		{"abstract",		BS2CC_TYFL_ABSTRACT},
		{"strict",			BS2CC_TYFL_STRICT},

//		{"byref",			BS2CC_TYFL_BYREF},
		{"delegate",		BS2CC_TYFL_DELEGATE},
		{"extern",			BS2CC_TYFL_EXTERN},
		{"const",			BS2CC_TYFL_CONST},
		{"inline",			BS2CC_TYFL_INLINE},
		{"thread",			BS2CC_TYFL_THREAD},
		{"dynamic",			BS2CC_TYFL_DYNAMIC},
		{"async",			BS2CC_TYFL_ASYNC},

		{"ltlendian",		BS2CC_TYFL_LTLENDIAN},
		{"bigendian",		BS2CC_TYFL_BIGENDIAN},
		{"dllimport",		BS2CC_TYFL_DLLIMPORT},
		{"dllexport",		BS2CC_TYFL_DLLEXPORT},
		{"typedef",			BS2CC_TYFL_TYPEDEF},
		{NULL,				0}
	};

	dtVal n0, n1, n2;
	char *t0, *t1, *t2;
	int fl;
	int i, j, k;

	fl=0;
	t0=BS2P_PeekToken(ctx, 0);

	while(t0 && (*t0=='I'))
	{
		for(i=0; modfl[i].name; i++)
		{
			if(!strcmp(t0+1, modfl[i].name))
			{
				fl|=modfl[i].fl;
				BS2P_NextToken(ctx);
				t0=BS2P_PeekToken(ctx, 0);
				break;
			}
		}
		if(!modfl[i].name)
			break;
	}

	if(!fl)
		return(DTV_NULL);

//	n0=BS2P_ParseWrapSimpleTagInt("modlst", fl);
	n0=dtvWrapLongF(fl);
	return(n0);
}

dtVal BS2P_ParseQName(BS2CC_CompileContext *ctx)
{
	char tb[256];
//	dtVal arr[64];
	dtVal n0, n1, n2;
	char *t0, *t1, *t2;
//	int ptrlvl, reflvl, arrlvl;

	t0=BS2P_PeekToken(ctx, 0);

	if(t0 && (*t0=='I'))
	{
		t2=tb;
		strcpy(t2, t0+1);
		t2+=strlen(t2);
		BS2P_NextToken(ctx);

		t0=BS2P_PeekToken(ctx, 0);
		t1=BS2P_PeekToken(ctx, 1);
		
		while(!strcmp(t0, "X.") && (*t1=='I'))
		{
			*t2++='/';
			strcpy(t2, t1+1);
			t2+=strlen(t2);

			BS2P_NextToken(ctx);
			BS2P_NextToken(ctx);

			t0=BS2P_PeekToken(ctx, 0);
			t1=BS2P_PeekToken(ctx, 1);
		}
	}

	return(BS2P_ParseWrapQName(ctx, tb));
}

dtVal BS2P_ParseQNameList(BS2CC_CompileContext *ctx)
{
	char tb[256];
	dtVal arr[64];
	dtVal n0, n1, n2;
	char *t0, *t1, *t2;
	int arrn;

	t0=BS2P_PeekToken(ctx, 0);

	arrn=0;
	while(t0 && (*t0=='I'))
	{
		n0=BS2P_ParseQName(ctx);
		arr[arrn++]=n0;
		if(BS2P_ParseExpectOptToken(ctx, "X,"))
		{
			t0=BS2P_PeekToken(ctx, 0);
			continue;
		}
		break;
	}
	
	if(arrn<=0)
		return(DTV_NULL);
	if(arrn==1)
		return(arr[0]);

	n0=BS2P_NewAstWrapArray(ctx, arr, arrn);
	return(n0);
}

dtVal BS2P_ParseTypeExpr(BS2CC_CompileContext *ctx)
{
	char tb[256];
	dtVal arr[64];
	dtVal n0, n1, n2;
	int i0, i1, i2;
	char *t0, *t1, *t2;
	int ptrlvl, reflvl, arrlvl;

	i0=BS2P_GetCurPosition(ctx);

	t0=BS2P_PeekToken(ctx, 0);

	ptrlvl=0; reflvl=0;
	while(t0)
	{
		if(!strcmp(t0, "X*"))
		{
			BS2P_NextToken(ctx);
			t0=BS2P_PeekToken(ctx, 0);
			ptrlvl++;
			continue;
		}

		if(!strcmp(t0, "X&"))
		{
			BS2P_NextToken(ctx);
			t0=BS2P_PeekToken(ctx, 0);
			reflvl++;
			continue;
		}
		
		break;
	}

	if(t0 && (*t0=='I') &&
		BS2P_DeclTypeRejectKeyword(ctx, t0+1))
	{
		BS2P_SetCurPosition(ctx, i0);
		return(DTV_NULL);
	}

	if(t0 && (*t0=='I'))
	{
		t2=tb;
		strcpy(t2, t0+1);
		t2+=strlen(t2);
		BS2P_NextToken(ctx);

		t0=BS2P_PeekToken(ctx, 0);
		t1=BS2P_PeekToken(ctx, 1);
		
		while(!strcmp(t0, "X.") && (*t1=='I'))
		{
			*t2++='/';
			strcpy(t2, t1+1);
			t2+=strlen(t2);

			BS2P_NextToken(ctx);
			BS2P_NextToken(ctx);

			t0=BS2P_PeekToken(ctx, 0);
			t1=BS2P_PeekToken(ctx, 1);
		}
	}else
	{
		BS2P_SetCurPosition(ctx, i0);
		return(DTV_NULL);
	}
	
	n0=BS2P_NewAstNode(ctx, "type");
	BS2P_SetAstNodeAttrS(n0, "name", tb);
	if(ptrlvl)
		BS2P_SetAstNodeAttrI(n0, "ptrlvl", ptrlvl);
	if(reflvl)
		BS2P_SetAstNodeAttrI(n0, "reflvl", reflvl);

	t0=BS2P_PeekToken(ctx, 0);

	arrlvl=0;
	while(t0)
	{
		if(!strcmp(t0, "X["))
		{
			BS2P_NextToken(ctx);

			n2=BS2P_ParseOptExpr(ctx);
			BS2P_ParseExpectToken(ctx, "X]");
			
			if(dtvTrueP(n2))
			{
				n1=BS2P_ParseWrapSimpleTagVal(ctx, "arrdef", n2);
//				n1=BS2P_ParseWrapSimpleTag(ctx, "arrdef");
//				if(dtvTrueP(n2))
//					BS2P_SetAstNodeAttr(n1, "size", n2);
				arr[arrlvl++]=n1;
			}else
			{
				n1=BS2P_ParseWrapSimpleTag(ctx, "arrdef0");
				arr[arrlvl++]=n1;
			}

			t0=BS2P_PeekToken(ctx, 0);
			continue;
		}

		if(!strcmp(t0, "X?"))
		{
			BS2P_SetAstNodeAttrI(n0, "nullable", 1);
			BS2P_NextToken(ctx);
			t0=BS2P_PeekToken(ctx, 0);
			continue;
		}

		if(!strcmp(t0, "X!"))
		{
			BS2P_SetAstNodeAttrI(n0, "nullable", 2);
			BS2P_NextToken(ctx);
			t0=BS2P_PeekToken(ctx, 0);
			continue;
		}

#if 0
		if(!strcmp(t0, "X*"))
		{
			BS2P_NextToken(ctx);
			t0=BS2P_PeekToken(ctx, 0);
			ptrlvl++;
			continue;
		}

		if(!strcmp(t0, "X&"))
		{
			BS2P_NextToken(ctx);
			t0=BS2P_PeekToken(ctx, 0);
			reflvl++;
			continue;
		}
#endif

		break;
	}
	
	if(arrlvl)
	{
		if(arrlvl>1)
		{
			n1=BS2P_NewAstWrapArray(ctx, arr, arrlvl);
			BS2P_SetAstNodeAttr(n0, "arrays", n1);
		}else
		{
			BS2P_SetAstNodeAttr(n0, "arrays", arr[0]);
		}
	}
	
	return(n0);
}

dtVal BS2P_TryParseDeclList(BS2CC_CompileContext *ctx,
	dtVal modif, dtVal tyexp)
{
	char tb[256];
	dtVal varlst[256];
	dtVal ty2;
	dtVal n0, n1, n2;
	char *tn;
	char *t0, *t1, *t2;
	int l0, l1, l2, l3;
	int nvars;
	int i, j, k;

	if(!dtvTrueP(tyexp))
		return(DTV_NULL);

	t0=BS2P_PeekToken(ctx, 0);
	t1=BS2P_PeekToken(ctx, 1);
	t2=BS2P_PeekToken(ctx, 2);

	if(!t0 || (*t0!='I'))
		return(DTV_NULL);
	
#if 1
	if(!strcmp(t0, "Iget") && (*t1=='I'))
	{
		tn=t1+1;
		sprintf(tb, "$GET$%s", tn);
		tn=BGBDT_TagStr_StrSymbol(tn);
		
		BS2P_NextToken(ctx);
		BS2P_NextToken(ctx);

//		n1=BS2P_ParseFunVars(ctx);
//		BS2P_ParseExpectToken(ctx, "X)");

		t0=BS2P_PeekToken(ctx, 0);
		if(!strcmp(t0, "X:"))
		{
			BS2P_NextToken(ctx);
			ty2=BS2P_ParseTypeExpr(ctx);
			t0=BS2P_PeekToken(ctx, 0);
		}

//		n2=BS2P_ParseBlockStatementTail2(ctx);

		l0=BS2P_GetCurPosition(ctx);
		n2=BS2P_ParseBlockStatementTail2(ctx);
		l1=BS2P_GetCurPosition(ctx);
		l2=l1-l0;

		n0=BS2P_NewAstNode(ctx, "func");
		BS2P_SetAstNodeAttrS(n0, "name", tn);
//			BS2P_SetAstNodeAttr(n0, "type", tyexp);
		BS2P_SetAstNodeAttr(n0, "type", ty2);
		if(dtvTrueP(modif))
			BS2P_SetAstNodeAttr(n0, "modi", modif);

//		if(dtvTrueP(n1))
//			BS2P_SetAstNodeAttr(n0, "args", n1);
		if(dtvTrueP(n2))
			BS2P_SetAstNodeAttr(n0, "body", n2);

		BS2P_SetAstNodeAttrI(n0, "tokcnt", l2);

//		varlst[nvars++]=n0;
//		break;
		return(n0);
	}

	if(!strcmp(t0, "Iset") && (*t1=='I') &&
		!strcmp(t2, "X("))
	{
		tn=t1+1;
		sprintf(tb, "$SET$%s", tn);
		tn=BGBDT_TagStr_StrSymbol(tn);

		BS2P_NextToken(ctx);
		BS2P_NextToken(ctx);
		BS2P_NextToken(ctx);

		n1=BS2P_ParseFunVars(ctx);
		BS2P_ParseExpectToken(ctx, "X)");

		t0=BS2P_PeekToken(ctx, 0);
		if(!strcmp(t0, "X:"))
		{
			BS2P_NextToken(ctx);
			ty2=BS2P_ParseTypeExpr(ctx);
			t0=BS2P_PeekToken(ctx, 0);
		}

//		n2=BS2P_ParseBlockStatementTail2(ctx);

		l0=BS2P_GetCurPosition(ctx);
		n2=BS2P_ParseBlockStatementTail2(ctx);
		l1=BS2P_GetCurPosition(ctx);
		l2=l1-l0;

		n0=BS2P_NewAstNode(ctx, "func");
		BS2P_SetAstNodeAttrS(n0, "name", tn);
//			BS2P_SetAstNodeAttr(n0, "type", tyexp);
		BS2P_SetAstNodeAttr(n0, "type", ty2);
		if(dtvTrueP(modif))
			BS2P_SetAstNodeAttr(n0, "modi", modif);

		if(dtvTrueP(n1))
			BS2P_SetAstNodeAttr(n0, "args", n1);
		if(dtvTrueP(n2))
			BS2P_SetAstNodeAttr(n0, "body", n2);

		BS2P_SetAstNodeAttrI(n0, "tokcnt", l2);

//		varlst[nvars++]=n0;	
//		break;
		return(n0);
	}
#endif

	if(BS2P_DeclTypeRejectKeyword(ctx, t0+1))
		return(DTV_NULL);
	
	nvars=0;
	while(t0 && (*t0=='I'))
	{
		tn=t0+1;
		BS2P_NextToken(ctx);

		t0=BS2P_PeekToken(ctx, 0);
		
		ty2=tyexp;
		if(!strcmp(t0, "X:"))
		{
			BS2P_NextToken(ctx);
			ty2=BS2P_ParseTypeExpr(ctx);

			t0=BS2P_PeekToken(ctx, 0);
		}

		if(!strcmp(t0, "X="))
		{
			BS2P_NextToken(ctx);
			n1=BS2P_ParseExpr(ctx);

			t0=BS2P_PeekToken(ctx, 0);
		}else
			{ n1=DTV_NULL; }
		
		if(!strcmp(t0, "X,") || !strcmp(t0, "X;"))
		{
			n0=BS2P_NewAstNode(ctx, "var");
			BS2P_SetAstNodeAttrS(n0, "name", tn);
//			BS2P_SetAstNodeAttr(n0, "type", tyexp);
			BS2P_SetAstNodeAttr(n0, "type", ty2);
			if(dtvTrueP(modif))
				BS2P_SetAstNodeAttr(n0, "modi", modif);

			if(dtvTrueP(n1))
				BS2P_SetAstNodeAttr(n0, "init", n1);

			varlst[nvars++]=n0;
			
			if(!strcmp(t0, "X,"))
			{
				BS2P_NextToken(ctx);
				t0=BS2P_PeekToken(ctx, 0);
				continue;
			}
			if(!strcmp(t0, "X;"))
			{
				BS2P_NextToken(ctx);
				break;
			}
			break;
		}

		if(!strcmp(t0, "X("))
		{
			BS2P_NextToken(ctx);
			n1=BS2P_ParseFunVars(ctx);
			BS2P_ParseExpectToken(ctx, "X)");

			t0=BS2P_PeekToken(ctx, 0);
			if(!strcmp(t0, "X:"))
			{
				BS2P_NextToken(ctx);
				ty2=BS2P_ParseTypeExpr(ctx);
				t0=BS2P_PeekToken(ctx, 0);
			}

			l0=BS2P_GetCurPosition(ctx);
			n2=BS2P_ParseBlockStatementTail2(ctx);
			l1=BS2P_GetCurPosition(ctx);
			l2=l1-l0;

			n0=BS2P_NewAstNode(ctx, "func");
			BS2P_SetAstNodeAttrS(n0, "name", tn);
//			BS2P_SetAstNodeAttr(n0, "type", tyexp);
			BS2P_SetAstNodeAttr(n0, "type", ty2);
			if(dtvTrueP(modif))
				BS2P_SetAstNodeAttr(n0, "modi", modif);

			if(dtvTrueP(n1))
				BS2P_SetAstNodeAttr(n0, "args", n1);
			if(dtvTrueP(n2))
				BS2P_SetAstNodeAttr(n0, "body", n2);

			BS2P_SetAstNodeAttrI(n0, "tokcnt", l2);

			varlst[nvars++]=n0;
			
			break;
		}
	}
	
	if(!nvars)
		return(DTV_NULL);
	
	if(nvars>1)
	{
		n1=BS2P_NewAstWrapArray(ctx, varlst, nvars);
		n0=BS2P_ParseWrapSimpleTagVal(ctx, "vars", n1);
		return(n0);
	}
	
	return(varlst[0]);
}

dtVal BS2P_TryParseDecl(BS2CC_CompileContext *ctx)
{
	dtVal modi, tyid, tvar, ty2;
	dtVal n0, n1, n2, n3, n4, n5;
	int l0, l1, l2, l3;
	char *t0, *t1, *t2;
	char *ocname, *fn;
	int i;
	
	i=BS2P_GetCurPosition(ctx);
	
	modi=BS2P_ParseModifierList(ctx);

	t0=BS2P_PeekToken(ctx, 0);
	t1=BS2P_PeekToken(ctx, 1);
	t2=BS2P_PeekToken(ctx, 2);

	if(t0 && (*t0=='I'))
	{
		if(ctx->cname && ((!strcmp(t0+1, ctx->cname) &&
				!strcmp(t1, "X(")) ||
			(!strcmp(t0, "X~") && !strcmp(t1+1, ctx->cname) &&
				!strcmp(t2, "X("))))
		{
			ty2=DTV_NULL;
			
			if(!strcmp(t0, "X~"))
			{
				BS2P_NextToken(ctx);
				BS2P_NextToken(ctx);
				fn="$DTOR$";
			}else
			{
				BS2P_NextToken(ctx);
				fn="$CTOR$";
			}

			BS2P_ParseExpectToken(ctx, "X(");
			n1=BS2P_ParseFunVars(ctx);
			BS2P_ParseExpectToken(ctx, "X)");

			t0=BS2P_PeekToken(ctx, 0);
			if(!strcmp(t0, "X:"))
			{
				BS2P_NextToken(ctx);
				ty2=BS2P_ParseTypeExpr(ctx);
				t0=BS2P_PeekToken(ctx, 0);
			}

//			n2=BS2P_ParseBlockStatement(ctx);

			l0=BS2P_GetCurPosition(ctx);
			n2=BS2P_ParseBlockStatementTail2(ctx);
			l1=BS2P_GetCurPosition(ctx);
			l2=l1-l0;

			n0=BS2P_NewAstNode(ctx, "func");
			BS2P_SetAstNodeAttrS(n0, "name", fn);
//			BS2P_SetAstNodeAttr(n0, "type", tyexp);
			if(dtvTrueP(ty2))
				BS2P_SetAstNodeAttr(n0, "type", ty2);
			if(dtvTrueP(modi))
				BS2P_SetAstNodeAttr(n0, "modi", modi);

			if(dtvTrueP(n1))
				BS2P_SetAstNodeAttr(n0, "args", n1);
			if(dtvTrueP(n2))
				BS2P_SetAstNodeAttr(n0, "body", n2);

			BS2P_SetAstNodeAttrI(n0, "tokcnt", l2);

			return(n0);
		}

		if(!strcmp(t0, "Ipackage"))
		{
			BS2P_NextToken(ctx);
			
			n1=BS2P_ParseQName(ctx);
			n2=BS2P_ParsePackageStatement(ctx);
//			i=0;

			n0=BS2P_NewAstNode(ctx, "package");
			
			if(dtvTrueP(modi))
				BS2P_SetAstNodeAttr(n0, "modi", modi);
			BS2P_SetAstNodeAttr(n0, "name", n1);
			BS2P_SetAstNodeAttr(n0, "body", n2);

			return(n0);
		}

		if(!strcmp(t0, "Iimport"))
		{
			BS2P_NextToken(ctx);
			
			n1=BS2P_ParseQName(ctx);
//			n2=BS2P_ParsePackageStatement(ctx);
//			i=0;

			BS2P_ParseExpectOptToken(ctx, "X;");

			n0=BS2P_NewAstNode(ctx, "import");
			
			if(dtvTrueP(modi))
				BS2P_SetAstNodeAttr(n0, "modi", modi);
			BS2P_SetAstNodeAttr(n0, "name", n1);
//			BS2P_SetAstNodeAttr(n0, "body", n2);

			return(n0);
		}
		
		if(!strcmp(t0, "Iclass") || !strcmp(t0, "Istruct") ||
			!strcmp(t0, "Iinterface"))
		{
			BS2P_NextToken(ctx);
			
			n1=BS2P_ParseQName(ctx);
			
			ocname=ctx->cname;
			ctx->cname=BGBDT_TagStr_GetUtf8(n1);

			if(BS2P_ParseExpectOptToken(ctx, "Iextends"))
			{
				n2=BS2P_ParseQNameList(ctx);
			}else n2=DTV_NULL;

			if(BS2P_ParseExpectOptToken(ctx, "Iimplements"))
			{
				n3=BS2P_ParseQNameList(ctx);
			}else n3=DTV_NULL;

			n4=BS2P_ParsePackageStatement(ctx);
//			i=0;

//			n0=BS2P_NewAstNode("class");
			n0=BS2P_NewAstNode(ctx, t0+1);
			
			if(dtvTrueP(modi))
				BS2P_SetAstNodeAttr(n0, "modi", modi);
			BS2P_SetAstNodeAttr(n0, "name", n1);

			if(dtvTrueP(n2))
				BS2P_SetAstNodeAttr(n0, "exts", n2);
			if(dtvTrueP(n3))
				BS2P_SetAstNodeAttr(n0, "impl", n3);

			BS2P_SetAstNodeAttr(n0, "body", n4);

			ctx->cname=ocname;
			return(n0);
		}
	}


	tyid=BS2P_ParseTypeExpr(ctx);
	tvar=BS2P_TryParseDeclList(ctx, modi, tyid);
	
	if(!dtvTrueP(tvar))
	{
		BS2P_SetCurPosition(ctx, i);
		return(DTV_NULL);
	}
	
	return(tvar);
}

dtVal BS2P_TryParseDeclSingle(BS2CC_CompileContext *ctx,
	dtVal modif, dtVal tyexp)
{
	dtVal varlst[256];
	dtVal n0, n1, n2;
	char *tn;
	char *t0, *t1, *t2;
	int nvars;
	int i, j, k;

	if(!dtvTrueP(tyexp))
		return(DTV_NULL);

	t0=BS2P_PeekToken(ctx, 0);

	if(!t0 || (*t0!='I'))
		return(DTV_NULL);
	
	nvars=0;

	tn=t0+1;
	BS2P_NextToken(ctx);

	t0=BS2P_PeekToken(ctx, 0);
	if(!strcmp(t0, "X="))
	{
		BS2P_NextToken(ctx);
		n1=BS2P_ParseExpr(ctx);
		t0=BS2P_PeekToken(ctx, 0);
	}else
		{ n1=DTV_NULL; }

	if(!strcmp(t0, "X("))
	{
		BS2P_NextToken(ctx);
		n1=BS2P_ParseFunVars(ctx);
		BS2P_ParseExpectToken(ctx, "X)");

		t0=BS2P_PeekToken(ctx, 0);
		if(!strcmp(t0, "X;"))
		{
			BS2P_NextToken(ctx);
			n2=DTV_NULL;
		}else
		{
			n2=BS2P_ParseBlockStatementTail(ctx);
		}

		n0=BS2P_NewAstNode(ctx, "func");
		BS2P_SetAstNodeAttrS(n0, "name", tn);
		BS2P_SetAstNodeAttr(n0, "type", tyexp);
		if(dtvTrueP(modif))
			BS2P_SetAstNodeAttr(n0, "modi", modif);

		if(dtvTrueP(n1))
			BS2P_SetAstNodeAttr(n0, "args", n1);
		if(dtvTrueP(n2))
			BS2P_SetAstNodeAttr(n0, "body", n2);

		varlst[nvars++]=n0;
	}else
	{
		n0=BS2P_NewAstNode(ctx, "var");
		BS2P_SetAstNodeAttrS(n0, "name", tn);
		BS2P_SetAstNodeAttr(n0, "type", tyexp);
		if(dtvTrueP(modif))
			BS2P_SetAstNodeAttr(n0, "modi", modif);

		if(dtvTrueP(n1))
			BS2P_SetAstNodeAttr(n0, "init", n1);

		varlst[nvars++]=n0;
			
		if(!strcmp(t0, "X,"))
		{
			BS2P_NextToken(ctx);
			t0=BS2P_PeekToken(ctx, 0);
		}else if(!strcmp(t0, "X;"))
		{
			BS2P_NextToken(ctx);
		}
	}
	return(varlst[0]);
}

dtVal BS2P_TryParseArgDecl(BS2CC_CompileContext *ctx)
{
	dtVal modi, tyid, tvar;
	dtVal n0, n1, n2;
	char *fn, *tn;
	char *t0, *t1, *t2;
	int i;
	
	i=BS2P_GetCurPosition(ctx);

	t0=BS2P_PeekToken(ctx, 0);
	t1=BS2P_PeekToken(ctx, 1);
	if((*t0=='I') && !strcmp(t1, "X:"))
	{
		fn=t0+1;
		BS2P_NextToken(ctx);
		BS2P_NextToken(ctx);

		tyid=BS2P_ParseTypeExpr(ctx);
		t0=BS2P_PeekToken(ctx, 0);

		if(!strcmp(t0, "X="))
		{
			BS2P_NextToken(ctx);
			n1=BS2P_ParseExpr(ctx);
			t0=BS2P_PeekToken(ctx, 0);
		}else
			{ n1=DTV_NULL; }

		n0=BS2P_NewAstNode(ctx, "var");
		BS2P_SetAstNodeAttrS(n0, "name", fn);
		BS2P_SetAstNodeAttr(n0, "type", tyid);
//		if(dtvTrueP(modif))
//			BS2P_SetAstNodeAttr(n0, "modi", modif);

		if(dtvTrueP(n1))
			BS2P_SetAstNodeAttr(n0, "init", n1);

		return(n0);
	}
	
	modi=BS2P_ParseModifierList(ctx);
	tyid=BS2P_ParseTypeExpr(ctx);
	tvar=BS2P_TryParseDeclSingle(ctx, modi, tyid);
	
	if(!dtvTrueP(tvar))
	{
		BS2P_SetCurPosition(ctx, i);
		return(DTV_NULL);
	}
	
	return(tvar);
}

dtVal BS2P_ParseFunVars(BS2CC_CompileContext *ctx)
{
	dtVal n0, n1, n2, n3;
	char *t0, *t1, *t2;
	dtVal vars[4096];
	int nvars;

	nvars=0;
	t0=BS2P_PeekToken(ctx, 0);
	while(t0 && *t0)
	{
		if(!strcmp(t0, "X)"))
			break;

		if(!strcmp(t0, "X..."))
		{
			n1=BS2P_NewAstNode(ctx, "vararg");
			vars[nvars++]=n1;

			BS2P_NextToken(ctx);
			t0=BS2P_PeekToken(ctx, 0);
			
			if(*t0=='I')
			{
				BS2P_SetAstNodeAttrS(n1, "name", t0+1);
				BS2P_NextToken(ctx);
				t0=BS2P_PeekToken(ctx, 0);
			}
			
			continue;
		}

		n1=BS2P_TryParseArgDecl(ctx);
		if(!dtvNullP(n1))
		{
			vars[nvars++]=n1;
			BS2P_ParseExpectOptToken(ctx, "X,");
			t0=BS2P_PeekToken(ctx, 0);
		}else
		{
			BS2P_ParseExpectToken(ctx, "X,");
			t0=BS2P_PeekToken(ctx, 0);
		}
	}
	
	if(nvars<1)
		return(DTV_NULL);
	
	n1=BS2P_NewAstWrapArray(ctx, vars, nvars);
	return(n1);
}
