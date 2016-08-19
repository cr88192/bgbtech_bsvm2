// #include <bteifgl.h>

s64 BS2C_ReduceClampConstRangeL(BS2CC_CompileContext *ctx, s64 iv, int dty)
{
	s64 ov;
	switch(dty)
	{
	case BSVM2_OPZ_INT:
		ov=(s32)iv;
		if(iv!=ov)
			BS2C_WarnConstRange(ctx);
		break;
	case BSVM2_OPZ_LONG:
		ov=(s64)iv;
		if(iv!=ov)
			BS2C_WarnConstRange(ctx);
		break;
	case BSVM2_OPZ_UINT:
		ov=(u32)iv;
		if(iv!=ov)
			BS2C_WarnConstRange(ctx);
		break;
	case BSVM2_OPZ_UBYTE:
		ov=(byte)iv;
		if(iv!=ov)
			BS2C_WarnConstRange(ctx);
		break;
	case BSVM2_OPZ_SHORT:
		ov=(s16)iv;
		if(iv!=ov)
			BS2C_WarnConstRange(ctx);
		break;
	case BSVM2_OPZ_SBYTE:
		ov=(sbyte)iv;
		if(iv!=ov)
			BS2C_WarnConstRange(ctx);
		break;
	case BSVM2_OPZ_USHORT:
		ov=(u16)iv;
		if(iv!=ov)
			BS2C_WarnConstRange(ctx);
		break;
	case BSVM2_OPZ_ULONG:
		ov=(u64)iv;
		if(iv!=ov)
			BS2C_WarnConstRange(ctx);
		break;

	default:
		BS2C_CaseError(ctx);
		break;
	}
	return(ov);
}

dtVal BS2C_ReduceExpr(BS2CC_CompileContext *ctx, dtVal expr)
{
	dtVal ln, rn, cn, tn;
	BGBDTC_X128 xa, xb, xc;
	s64 li, lj, lk;
	double lf, lg, lh;
	int lt, rt, cty;
	char *tag, *op;
	int i;

	if(dtvIsFixIntP(expr))
		{ return(expr); }
	if(dtvIsFixUIntP(expr))
		{ return(expr); }
	if(dtvIsFixLongP(expr))
		{ return(expr); }
	if(dtvIsFixFloatP(expr))
		{ return(expr); }
	if(dtvIsFixDoubleP(expr))
		{ return(expr); }
	if(BGBDT_TagStr_IsSymbolP(expr))
		{ return(expr); }

	tag=BS2P_GetAstNodeTag(expr);
	
	if(!tag)
		{ return(expr); }

	if(!strcmp(tag, "binary"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
		op=BS2P_GetAstNodeAttrS(expr, "op");

		ln=BS2C_ReduceExpr(ctx, ln);
		rn=BS2C_ReduceExpr(ctx, rn);

		lt=BS2C_InferExpr(ctx, ln);
		rt=BS2C_InferExpr(ctx, rn);
//		cty=BS2C_InferSuperType(ctx, lt, rt);
		cty=BS2C_TypeBinarySuperType(ctx, op, lt, rt);

		if(BS2C_TypeSmallLongP(ctx, cty) &&
			dtvIsSmallLongP(ln) && dtvIsSmallLongP(rn))
		{
			li=dtvUnwrapLong(ln);
			lj=dtvUnwrapLong(rn);
			lk=0;
			if(!strcmp(op, "+"))lk=li+lj;
			else if(!strcmp(op, "-"))lk=li-lj;
			else if(!strcmp(op, "*"))lk=li*lj;
			else if(!strcmp(op, "&"))lk=li&lj;
			else if(!strcmp(op, "|"))lk=li|lj;
			else if(!strcmp(op, "^"))lk=li^lj;
			else if(!strcmp(op, "<<"))
				{ i=(u16)lj; lk=(i<64)?(li<<i):0; }
			else if(!strcmp(op, ">>"))
			{
				i=(u16)lj;
				if(BS2C_TypeUnsignedP(ctx, lt))
					{ lk=(i<64)?(((u64)li)>>i):0; }
				else
					{ lk=(i<64)?(li>>i):(li>>63); }
			}
			else if(!strcmp(op, ">>>"))
				{ i=(u16)lj; lk=(i<64)?(((u64)li)>>i):0; }
			else if(!strcmp(op, "/") || !strcmp(op, "%"))
			{
				if(lj==0)
				{
					BS2C_ErrDivZero(ctx);
					return(expr);
				}
				if(!strcmp(op, "/"))lk=li/lj;
				if(!strcmp(op, "%"))lk=li%lj;
			}else
			{
				return(expr);
			}
			
			lk=BS2C_ReduceClampConstRangeL(ctx, lk, cty);			
			if(BS2C_TypeSmallIntP(ctx, cty))
				{ tn=dtvWrapInt(lk); }
			else
				{ tn=dtvWrapLong(lk); }
			return(tn);
		}

		if(BS2C_TypeSmallDoubleP(ctx, cty) &&
			dtvIsSmallDoubleP(ln) && dtvIsSmallDoubleP(rn))
		{
			lf=dtvUnwrapDouble(ln);
			lg=dtvUnwrapDouble(rn);
			lh=0;
			if(!strcmp(op, "+"))lh=lf+lg;
			else if(!strcmp(op, "-"))lh=lf-lg;
			else if(!strcmp(op, "*"))lh=lf*lg;
			else if(!strcmp(op, "/"))
			{
				if(lg==0)
				{
					BS2C_ErrDivZero(ctx);
					return(expr);
				}
				lh=lf/lg;
			}else
			{
				return(expr);
			}
			
//			lk=BS2C_ReduceClampConstRangeL(ctx, lk, cty);			
			if(BS2C_TypeSmallFloatP(ctx, cty))
				{ tn=dtvWrapFloat(lh);}
			else
				{ tn=dtvWrapDouble(lh); }
			return(tn);
		}

		if(BS2C_TypeSmallInt128P(ctx, cty) &&
			BGBDT_XI128_IsSmallInt128P(ln) &&
			BGBDT_XI128_IsSmallInt128P(rn))
		{
//			li=dtvUnwrapLong(ln);
//			lj=dtvUnwrapLong(rn);
			xa=BGBDT_XI128_UnwrapInt128(ln);
			xb=BGBDT_XI128_UnwrapInt128(rn);

			lk=0;
			if(!strcmp(op, "+"))
				{ xc=BGBDT_XI128_AddX(xa, xb); }
			else if(!strcmp(op, "-"))
				{ xc=BGBDT_XI128_SubX(xa, xb); }
			else if(!strcmp(op, "*"))
				{ xc=BGBDT_XI128_MulX(xa, xb); }
			else if(!strcmp(op, "&"))
				{ xc=BGBDT_XI128_AndX(xa, xb); }
			else if(!strcmp(op, "|"))
				{ xc=BGBDT_XI128_OrX(xa, xb); }
			else if(!strcmp(op, "^"))
				{ xc=BGBDT_XI128_XorX(xa, xb); }
			else if(!strcmp(op, "<<"))
				{ xc=BGBDT_XI128_ShlX(xa, xb.a); }
			else if(!strcmp(op, ">>"))
			{
				if(BS2C_TypeUnsignedP(ctx, lt))
					{ xc=BGBDT_XI128_ShrX(xa, xb.a); }
				else
					{ xc=BGBDT_XI128_SarX(xa, xb.a); }
			}
			else if(!strcmp(op, ">>>"))
				{ xc=BGBDT_XI128_ShrX(xa, xb.a); }
			else if(!strcmp(op, "/") || !strcmp(op, "%"))
			{
				if(BGBDT_XI128_ZeroXP(xb))
				{
					BS2C_ErrDivZero(ctx);
					return(expr);
				}
				if(!strcmp(op, "/"))
					{ xc=BGBDT_XI128_DivX(xa, xb); }
				if(!strcmp(op, "%"))
					{ xc=BGBDT_XI128_ModX(xa, xb); }
			}else
			{
				return(expr);
			}
			
//			lk=BS2C_ReduceClampConstRangeL(ctx, lk, cty);			
//			if(BS2C_TypeSmallIntP(ctx, cty))
//				{ tn=dtvWrapInt(lk); }
//			else
//				{ tn=dtvWrapLong(lk); }
			tn=BGBDT_XI128_WrapInt128(xc);
			return(tn);
		}

		if(BS2C_TypeSmallFloat128P(ctx, cty) &&
			BGBDT_XF128_IsSmallFloat128P(ln) &&
			BGBDT_XF128_IsSmallFloat128P(rn))
		{
			xa=BGBDT_XF128_UnwrapFloat128(ln);
			xb=BGBDT_XF128_UnwrapFloat128(rn);

			if(!strcmp(op, "+"))
				{ xc=BGBDT_XF128_AddX(xa, xb); }
			else if(!strcmp(op, "-"))
				{ xc=BGBDT_XF128_SubX(xa, xb); }
			else if(!strcmp(op, "*"))
				{ xc=BGBDT_XF128_MulX(xa, xb); }
			else if(!strcmp(op, "/"))
			{
				if(BGBDT_XF128_ToDouble(xb)==0.0)
				{
					BS2C_ErrDivZero(ctx);
					return(expr);
				}
				xc=BGBDT_XF128_DivX(xa, xb);
			}else
			{
				return(expr);
			}
			
			tn=BGBDT_XF128_WrapFloat128(xc);
			return(tn);
		}

		return(expr);
	}

	if(!strcmp(tag, "bincmp"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
		op=BS2P_GetAstNodeAttrS(expr, "op");

		ln=BS2C_ReduceExpr(ctx, ln);
		rn=BS2C_ReduceExpr(ctx, rn);

		lt=BS2C_InferExpr(ctx, ln);
		rt=BS2C_InferExpr(ctx, rn);
//		cty=BS2C_InferSuperType(ctx, lt, rt);
		cty=BS2C_TypeCompareSuperType(ctx, lt, rt);

		if(BS2C_TypeSmallLongP(ctx, cty) &&
			dtvIsSmallLongP(ln) && dtvIsSmallLongP(rn))
		{
			li=dtvUnwrapLong(ln);
			lj=dtvUnwrapLong(rn);
			if(!strcmp(op, "=="))i=(li==lj);
			else if(!strcmp(op, "!="))i=(li!=lj);
			else if(!strcmp(op, "<"))i=(li<lj);
			else if(!strcmp(op, ">"))i=(li>lj);
			else if(!strcmp(op, "<="))i=(li<=lj);
			else if(!strcmp(op, ">="))i=(li>=lj);
			else if(!strcmp(op, "==="))i=(li==lj);
			else if(!strcmp(op, "!=="))i=(li!=lj);
			else
			{
				return(expr);
			}
			
			tn=dtvWrapInt(i);
			return(tn);
		}

		if(BS2C_TypeSmallDoubleP(ctx, cty) &&
			dtvIsSmallDoubleP(ln) && dtvIsSmallDoubleP(rn))
		{
			lf=dtvUnwrapDouble(ln);
			lg=dtvUnwrapDouble(rn);
			if(!strcmp(op, "=="))i=(lf==lg);
			else if(!strcmp(op, "!="))i=(lf!=lg);
			else if(!strcmp(op, "<"))i=(lf<lg);
			else if(!strcmp(op, ">"))i=(lf>lg);
			else if(!strcmp(op, "<="))i=(lf<=lg);
			else if(!strcmp(op, ">="))i=(lf>=lg);
			{
				return(expr);
			}
			
			tn=dtvWrapInt(i);
			return(tn);
		}

		return(expr);
	}

	if(!strcmp(tag, "tern"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
		cn=BS2P_GetAstNodeAttr(expr, "cond");

		ln=BS2C_ReduceExpr(ctx, ln);
		rn=BS2C_ReduceExpr(ctx, rn);
		cn=BS2C_ReduceExpr(ctx, cn);
		
		i=BS2C_CompileGetCondBool(ctx, cn);
		if(i==1)return(ln);
		if(i==0)return(rn);
		return(expr);
	}

	if(!strcmp(tag, "instanceof"))
	{
		return(expr);
	}

	if(!strcmp(tag, "cast") || !strcmp(tag, "cast_strict"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");

		ln=BS2C_ReduceExpr(ctx, ln);
//		rn=BS2C_ReduceExpr(ctx, rn);

		cty=BS2C_TypeBaseType(ctx, rn);

		if(BS2C_TypeSmallLongP(ctx, cty) && dtvIsSmallLongP(ln))
		{
			li=dtvUnwrapLong(ln);
			lk=BS2C_ReduceClampConstRangeL(ctx, li, cty);
			if(BS2C_TypeSmallIntP(ctx, cty))
				{ tn=dtvWrapInt(lk); }
			else
				{ tn=dtvWrapLong(lk); }
			return(tn);
		}

		return(expr);
	}

	return(expr);
}