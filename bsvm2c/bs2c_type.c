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


int BS2C_GetTypeBaseZ(BS2CC_CompileContext *ctx, int ty)
{
	if((ty>=0) && (ty<16))
		return(ty);
	if(ty>=256)
		return(BSVM2_OPZ_ADDRESS);
	
	if(ty==BS2CC_TYZ_VARIANT)
		return(BSVM2_OPZ_ADDRESS);
	if(ty==BS2CC_TYZ_STRING)
		return(BSVM2_OPZ_ADDRESS);
	if(ty==BS2CC_TYZ_CHAR)
		return(BSVM2_OPZ_USHORT);
	if(ty==BS2CC_TYZ_CHAR8)
		return(BSVM2_OPZ_UBYTE);
	if(ty==BS2CC_TYZ_NLONG)
		return(BSVM2_OPZ_LONG);
	if(ty==BS2CC_TYZ_UNLONG)
		return(BSVM2_OPZ_ULONG);
	if(ty==BS2CC_TYZ_BOOL)
		return(BSVM2_OPZ_UBYTE);
	if(ty==BS2CC_TYZ_HFLOAT)
		return(BSVM2_OPZ_USHORT);
	if(ty==BS2CC_TYZ_AUTOVAR)
		return(BSVM2_OPZ_ADDRESS);

//	if(ty==BS2CC_TYZ_VEC2F)
//		return(BSVM2_OPZ_LONG);

	return(BSVM2_OPZ_ADDRESS);
}

int BS2C_GetTypeVecZ(BS2CC_CompileContext *ctx, int ty)
{
	if(ty==BS2CC_TYZ_VEC2F)
		return(BSVM2_OPVZ_V2F);
	if(ty==BS2CC_TYZ_VEC3F)
		return(BSVM2_OPVZ_V3F);
	if(ty==BS2CC_TYZ_VEC4F)
		return(BSVM2_OPVZ_V4F);
	if(ty==BS2CC_TYZ_VEC2D)
		return(BSVM2_OPVZ_V2D);
	if(ty==BS2CC_TYZ_QUATF)
		return(BSVM2_OPVZ_V4F);
	if(ty==BS2CC_TYZ_FCPLX)
		return(BSVM2_OPVZ_V2F);
	if(ty==BS2CC_TYZ_DCPLX)
		return(BSVM2_OPVZ_V2D);

	if(ty==BS2CC_TYZ_VEC3D)
		return(BSVM2_OPVZ_V3D);
	if(ty==BS2CC_TYZ_VEC4D)
		return(BSVM2_OPVZ_V4D);
	if(ty==BS2CC_TYZ_VEC3XF)
		return(BSVM2_OPVZ_V3XF);

	if(ty==BS2CC_TYZ_INT128)
		return(BSVM2_OPVZ_I128);
	if(ty==BS2CC_TYZ_UINT128)
		return(BSVM2_OPVZ_UI128);
	if(ty==BS2CC_TYZ_FLOAT128)
		return(BSVM2_OPVZ_F128);

	return(-1);
}

char *BS2C_GetTypeSig(BS2CC_CompileContext *ctx, int ty)
{
	char tb[256];
	BS2CC_TypeOverflow tovf;
	BS2CC_TypeOverflow *ovf;
	BS2CC_VarInfo *vi;
	char *t;
	int al, asz, bty;
	int i, j, k;

	if((ty&BS2CC_TYT_MASK)==BS2CC_TYT_BASIC)
	{
		al =(ty>>BS2CC_TYI_SHR)&BS2CC_TYI_MASK2;
		asz=(ty>>BS2CC_TYS_SHR)&BS2CC_TYS_MASK2;
		bty=(ty>>BS2CC_TYE_SHR)&BS2CC_TYE_MASK2;
		
		ovf=&tovf;
		memset(ovf, 0, sizeof(BS2CC_TypeOverflow));

		if((al>=1) && (al<=3))
		{
			ovf->al=al;
		}else if((al>=5) && (al<=7))
		{
			i=al-4;
			ovf->pl=i;
		}else if(al==4)
		{
			ovf->rf=1;
		}
		
		ovf->base=bty;
//		ovf->al=al;
		if(asz)
		{
			ovf->asz[0]=asz;
			ovf->an=1;
		}
	}else if((ty&BS2CC_TYT_MASK)==BS2CC_TYT_OVF)
	{
		ovf=ctx->tyovf[ty&BS2CC_TYO_MASK];
		al=ovf->al;
		bty=ovf->base;
	}else
	{
		BSVM2_DBGTRAP
		return(NULL);
	}
	
	t=tb;

	if(ovf->rf)
	{
		*t++='R';
		*t=0;
	}

#if 0
	if(asz)
	{
		sprintf(t, "A%d", asz);
		t+=strlen(t);
	}
#endif

	switch(ovf->an)
	{
	case 0:
		break;
	case 1:
		sprintf(t, "A%d", ovf->asz[0]);
		t+=strlen(t);
		break;
	case 2:
		sprintf(t, "A%d,%d;", ovf->asz[0], ovf->asz[1]);
		t+=strlen(t);
		break;
	case 3:
		sprintf(t, "A%d,%d,%d;", ovf->asz[0], ovf->asz[1], ovf->asz[2]);
		t+=strlen(t);
		break;
	case 4:
		sprintf(t, "A%d,%d,%d,%d;", ovf->asz[0],
			ovf->asz[1], ovf->asz[2], ovf->asz[3]);
		t+=strlen(t);
		break;
	default:
		BSVM2_DBGTRAP
		break;
	}

	if(ovf->al>ovf->an)
	{
		i=ovf->al-ovf->an;
		while(i--)
			*t++='Q';
		*t=0;
	}
	
	if(ovf->pl)
	{
		i=ovf->pl;
		while(i--)
			*t++='P';
		*t=0;
	}

#if 0
	if((al>=1) && (al<=3))
	{
		i=al;
		while(i--)
			*t++='Q';
		*t=0;
	}else if((al>=5) && (al<=7))
	{
		i=al-4;
		while(i--)
			*t++='P';
		*t=0;
	}else if(al==4)
	{
		*t++='R';
	}
#endif

	if(bty<256)
	{
		switch(bty)
		{
		case BS2CC_TYZ_INT:		*t++='i'; break;
		case BS2CC_TYZ_LONG:	*t++='x'; break;
		case BS2CC_TYZ_FLOAT:	*t++='f'; break;
		case BS2CC_TYZ_DOUBLE:	*t++='d'; break;
		case BS2CC_TYZ_ADDRESS:	*t++='r'; break;
		case BS2CC_TYZ_UINT:	*t++='j'; break;
		case BS2CC_TYZ_UBYTE:	*t++='h'; break;
		case BS2CC_TYZ_SHORT:	*t++='s'; break;
		case BS2CC_TYZ_SBYTE:	*t++='a'; break;
		case BS2CC_TYZ_USHORT:	*t++='t'; break;
		case BS2CC_TYZ_ULONG:	*t++='y'; break;
		case BS2CC_TYZ_NLONG:	*t++='l'; break;
		case BS2CC_TYZ_UNLONG:	*t++='m'; break;
		case BS2CC_TYZ_VOID:	*t++='v'; break;
		case BS2CC_TYZ_VARIANT:	*t++='r'; break;
		case BS2CC_TYZ_VARARG:	*t++='z'; break;
		case BS2CC_TYZ_CHAR:	*t++='w'; break;
		case BS2CC_TYZ_CHAR8:	*t++='c'; break;
		case BS2CC_TYZ_STRING:	*t++='C'; *t++='s'; break;
		case BS2CC_TYZ_CSTRING:	*t++='P'; *t++='c'; break;
		case BS2CC_TYZ_BOOL:	*t++='b'; break;
		case BS2CC_TYZ_HFLOAT:	*t++='k'; break;

		case BS2CC_TYZ_AUTOVAR:	*t++='C'; *t++='r'; break;

		case BS2CC_TYZ_SYMBOL:		*t++='C'; *t++='t'; break;
		case BS2CC_TYZ_KEYWORD:		*t++='C'; *t++='n'; break;

		case BS2CC_TYZ_INT128:		*t++='n'; break;
		case BS2CC_TYZ_UINT128:		*t++='o'; break;
		case BS2CC_TYZ_FLOAT128:	*t++='g'; break;

		case BS2CC_TYZ_VEC2F:	*t++='C'; *t++='a'; break;
		case BS2CC_TYZ_VEC3F:	*t++='C'; *t++='b'; break;
		case BS2CC_TYZ_VEC4F:	*t++='C'; *t++='c'; break;
		case BS2CC_TYZ_VEC2D:	*t++='C'; *t++='e'; break;
		case BS2CC_TYZ_QUATF:	*t++='C'; *t++='q'; break;
		case BS2CC_TYZ_FCPLX:	*t++='C'; *t++='f'; break;
		case BS2CC_TYZ_DCPLX:	*t++='C'; *t++='d'; break;
		case BS2CC_TYZ_VEC3XF:	*t++='C'; *t++='h'; break;

		case BS2CC_TYZ_VEC3D:	*t++='D'; *t++='b'; break;
		case BS2CC_TYZ_VEC4D:	*t++='D'; *t++='c'; break;

		default:				*t++='r'; break;
		}
		*t=0;
	}else
	{
		vi=ctx->globals[bty-256];
		switch(vi->vitype)
		{
		case BS2CC_VITYPE_STRUCT:
			sprintf(t, "X%d", bty-256);
			break;
		case BS2CC_VITYPE_CLASS:
		case BS2CC_VITYPE_IFACE:
			sprintf(t, "L%d", bty-256);
			break;
		default:
			sprintf(t, "L%d", bty-256);
			break;
		}
	}
	return(bgbdt_mm_rstrdup(tb));
}

char *BS2C_GetTypeNameStr(BS2CC_CompileContext *ctx, int ty)
{
	char tb[256];
	BS2CC_TypeOverflow tovf;
	BS2CC_TypeOverflow *ovf;
	BS2CC_VarInfo *vi;
	char *t;
	int al, asz, bty;
	int i, j, k;
	
	if((ty&BS2CC_TYT_MASK)==BS2CC_TYT_BASIC)
	{
		al =(ty>>BS2CC_TYI_SHR)&BS2CC_TYI_MASK2;
		asz=(ty>>BS2CC_TYS_SHR)&BS2CC_TYS_MASK2;
		bty=(ty>>BS2CC_TYE_SHR)&BS2CC_TYE_MASK2;
		
		ovf=&tovf;
		memset(ovf, 0, sizeof(BS2CC_TypeOverflow));

		if((al>=1) && (al<=3))
		{
			ovf->al=al;
		}else if((al>=5) && (al<=7))
		{
			i=al-4;
			ovf->pl=i;
		}else if(al==4)
		{
			ovf->rf=1;
		}
		
		ovf->base=bty;
//		ovf->al=al;
		if(asz)
		{
			ovf->asz[0]=asz;
			ovf->an=1;
		}
	}else if((ty&BS2CC_TYT_MASK)==BS2CC_TYT_OVF)
	{
		ovf=ctx->tyovf[ty&BS2CC_TYO_MASK];
		al=ovf->al;
		bty=ovf->base;
	}else
	{
		BSVM2_DBGTRAP
		return(NULL);
	}
	
	t=tb;

	if(ovf->rf)
	{
		*t++='&';
		*t=0;
	}

#if 0
	if(asz)
	{
		sprintf(t, "A%d", asz);
		t+=strlen(t);
	}
#endif
	
	if(ovf->pl)
	{
		i=ovf->pl;
		while(i--)
			*t++='*';
		*t=0;
	}

#if 0
	if((al>=1) && (al<=3))
	{
		i=al;
		while(i--)
		{
			*t++='[';
			*t++=']';
		}
		*t=0;
	}else if((al>=5) && (al<=7))
	{
		i=al-4;
		while(i--)
			*t++='*';
		*t=0;
	}else if(al==4)
	{
		*t++='&';
	}
#endif	

	if(bty<256)
	{
		switch(bty)
		{
		case BS2CC_TYZ_INT:
			strcpy(t, "int"); t+=strlen(t); break;
		case BS2CC_TYZ_LONG:
			strcpy(t, "long"); t+=strlen(t); break;
		case BS2CC_TYZ_FLOAT:
			strcpy(t, "float"); t+=strlen(t); break;
		case BS2CC_TYZ_DOUBLE:
			strcpy(t, "double"); t+=strlen(t); break;
		case BS2CC_TYZ_ADDRESS:
			strcpy(t, "address"); t+=strlen(t); break;
		case BS2CC_TYZ_UINT:
			strcpy(t, "uint"); t+=strlen(t); break;
		case BS2CC_TYZ_UBYTE:
			strcpy(t, "byte"); t+=strlen(t); break;
		case BS2CC_TYZ_SHORT:
			strcpy(t, "short"); t+=strlen(t); break;
		case BS2CC_TYZ_SBYTE:
			strcpy(t, "sbyte"); t+=strlen(t); break;
		case BS2CC_TYZ_USHORT:
			strcpy(t, "ushort"); t+=strlen(t); break;
		case BS2CC_TYZ_ULONG:
			strcpy(t, "ulong"); t+=strlen(t); break;
		case BS2CC_TYZ_NLONG:
			strcpy(t, "nlong"); t+=strlen(t); break;
		case BS2CC_TYZ_UNLONG:
			strcpy(t, "unlong"); t+=strlen(t); break;
		case BS2CC_TYZ_VOID:
			strcpy(t, "void"); t+=strlen(t); break;
		case BS2CC_TYZ_VARIANT:
			strcpy(t, "variant"); t+=strlen(t); break;
		case BS2CC_TYZ_VARARG:
			strcpy(t, "vararg"); t+=strlen(t); break;
		case BS2CC_TYZ_CHAR:
			strcpy(t, "char"); t+=strlen(t); break;
		case BS2CC_TYZ_CHAR8:
			strcpy(t, "char8"); t+=strlen(t); break;
		case BS2CC_TYZ_STRING:
			strcpy(t, "string"); t+=strlen(t); break;
		case BS2CC_TYZ_CSTRING:
			strcpy(t, "cstring"); t+=strlen(t); break;
		case BS2CC_TYZ_BOOL:
			strcpy(t, "bool"); t+=strlen(t); break;
		case BS2CC_TYZ_HFLOAT:
			strcpy(t, "hfloat"); t+=strlen(t); break;
		case BS2CC_TYZ_AUTOVAR:
			strcpy(t, "auto"); t+=strlen(t); break;
		case BS2CC_TYZ_SYMBOL:
			strcpy(t, "symbol"); t+=strlen(t); break;
		case BS2CC_TYZ_KEYWORD:
			strcpy(t, "keyword"); t+=strlen(t); break;

		case BS2CC_TYZ_INT128:
			strcpy(t, "int128"); t+=strlen(t); break;
		case BS2CC_TYZ_UINT128:
			strcpy(t, "uint128"); t+=strlen(t); break;
		case BS2CC_TYZ_FLOAT128:
			strcpy(t, "float128"); t+=strlen(t); break;

		case BS2CC_TYZ_VEC2F:
			strcpy(t, "vec2f"); t+=strlen(t); break;
		case BS2CC_TYZ_VEC3F:
			strcpy(t, "vec3f"); t+=strlen(t); break;
		case BS2CC_TYZ_VEC4F:
			strcpy(t, "vec4f"); t+=strlen(t); break;
		case BS2CC_TYZ_VEC2D:
			strcpy(t, "vec2d"); t+=strlen(t); break;
		case BS2CC_TYZ_QUATF:
			strcpy(t, "quatf"); t+=strlen(t); break;
		case BS2CC_TYZ_FCPLX:
			strcpy(t, "fcomplex"); t+=strlen(t); break;
		case BS2CC_TYZ_DCPLX:
			strcpy(t, "dcomplex"); t+=strlen(t); break;

		case BS2CC_TYZ_VEC3XF:
			strcpy(t, "vec3xf"); t+=strlen(t); break;
		case BS2CC_TYZ_VEC3D:
			strcpy(t, "vec3d"); t+=strlen(t); break;
		case BS2CC_TYZ_VEC4D:
			strcpy(t, "vec4d"); t+=strlen(t); break;

		default:
			strcpy(t, "unknown"); t+=strlen(t); break;
		}
		*t=0;
	}else
	{
		vi=ctx->globals[bty-256];
		switch(vi->vitype)
		{
		case BS2CC_VITYPE_STRUCT:
			sprintf(t, "X%d", bty-256);
			t+=strlen(t);
			break;
		case BS2CC_VITYPE_CLASS:
		case BS2CC_VITYPE_IFACE:
			sprintf(t, "L%d", bty-256);
			t+=strlen(t);
			break;
		default:
			sprintf(t, "L%d", bty-256);
			t+=strlen(t);
			break;
		}
	}

#if 0
	if(asz)
	{
		sprintf(t, "[%d]", asz);
		t+=strlen(t);
	}
#endif

#if 1
	switch(ovf->an)
	{
	case 0:
		break;
	case 1:
		sprintf(t, "[%d]", ovf->asz[0]);
		t+=strlen(t);
		break;
	case 2:
		sprintf(t, "[%d,%d]", ovf->asz[0], ovf->asz[1]);
		t+=strlen(t);
		break;
	case 3:
		sprintf(t, "[%d,%d,%d]", ovf->asz[0], ovf->asz[1], ovf->asz[2]);
		t+=strlen(t);
		break;
	case 4:
		sprintf(t, "[%d,%d,%d,%d]", ovf->asz[0],
			ovf->asz[1], ovf->asz[2], ovf->asz[3]);
		t+=strlen(t);
		break;
	default:
		BSVM2_DBGTRAP
		break;
	}
#endif

	if(ovf->al>ovf->an)
	{
		i=ovf->al-ovf->an;
		while(i--)
		{
			*t++='[';
			*t++=']';
		}
		*t=0;
	}

	return(bgbdt_mm_rstrdup(tb));
}


BS2CC_VarInfo *BS2C_GetTypeObject(BS2CC_CompileContext *ctx, int ty)
{
	BS2CC_TypeOverflow *ovf;
	BS2CC_VarInfo *vi;
	int bty;

	if(ty<0)
		return(NULL);

	if((ty&BS2CC_TYT_MASK)==BS2CC_TYT_BASIC)
	{
		bty=(ty>>BS2CC_TYE_SHR)&BS2CC_TYE_MASK2;
		if(bty>=256)
		{
			vi=ctx->globals[bty-256];
			return(vi);
		}
		return(NULL);
	}

	if((ty&BS2CC_TYT_MASK)==BS2CC_TYT_OVF)
	{
		ovf=ctx->tyovf[ty&BS2CC_TYO_MASK];
		bty=ovf->base;
		
		if(bty>=256)
		{
			vi=ctx->globals[bty-256];
			return(vi);
		}
		return(NULL);
	}
	
	return(NULL);
}

int BS2C_TypeFixedStructP(BS2CC_CompileContext *ctx, int ty)
{
	BS2CC_VarInfo *vi;

	vi=BS2C_GetTypeObject(ctx, ty);
	if(!vi)return(0);
	
	if(BS2C_TypeArrayP(ctx, ty))
		return(0);
	if(BS2C_TypePointerP(ctx, ty))
		return(0);
	
	return(vi->vitype==BS2CC_VITYPE_STRUCT);
}

int BS2C_TypeClassP(BS2CC_CompileContext *ctx, int ty)
{
	BS2CC_VarInfo *vi;

	vi=BS2C_GetTypeObject(ctx, ty);
	if(!vi)return(0);
	
	if(BS2C_TypeArrayP(ctx, ty))
		return(0);
	if(BS2C_TypePointerP(ctx, ty))
		return(0);
	
	return(vi->vitype==BS2CC_VITYPE_CLASS);
}

int BS2C_TypeInterfaceP(BS2CC_CompileContext *ctx, int ty)
{
	BS2CC_VarInfo *vi;

	vi=BS2C_GetTypeObject(ctx, ty);
	if(!vi)return(0);
	
	if(BS2C_TypeArrayP(ctx, ty))
		return(0);
	if(BS2C_TypePointerP(ctx, ty))
		return(0);
	
	return(vi->vitype==BS2CC_VITYPE_IFACE);
}

int BS2C_TypeClassOrInterfaceP(BS2CC_CompileContext *ctx, int ty)
{
	BS2CC_VarInfo *vi;

	vi=BS2C_GetTypeObject(ctx, ty);
	if(!vi)return(0);
	
	if(BS2C_TypeArrayP(ctx, ty))
		return(0);
	if(BS2C_TypePointerP(ctx, ty))
		return(0);
	
	return((vi->vitype==BS2CC_VITYPE_CLASS) ||
		(vi->vitype==BS2CC_VITYPE_IFACE));
}

int BS2C_TypeFunctionP(BS2CC_CompileContext *ctx, int ty)
{
	BS2CC_VarInfo *vi;

	vi=BS2C_GetTypeObject(ctx, ty);
	if(!vi)return(0);
	
	if(BS2C_TypeArrayP(ctx, ty))
		return(0);
	if(BS2C_TypePointerP(ctx, ty))
		return(0);
	
	return(vi->vitype==BS2CC_VITYPE_GBLFUNC);

//	return((vi->vitype==BS2CC_VITYPE_CLASS) ||
//		(vi->vitype==BS2CC_VITYPE_IFACE));
}

int BS2C_TypeSmallIntP(BS2CC_CompileContext *ctx, int ty)
{
	if((ty==BS2CC_TYZ_INT) || (ty==BS2CC_TYZ_UINT))
		return(1);
	if((ty==BS2CC_TYZ_SBYTE) || (ty==BS2CC_TYZ_UBYTE))
		return(1);
	if((ty==BS2CC_TYZ_SHORT) || (ty==BS2CC_TYZ_USHORT))
		return(1);
	if((ty==BS2CC_TYZ_CHAR) || (ty==BS2CC_TYZ_CHAR8))
		return(1);
	if(ty==BS2CC_TYZ_BOOL)
		return(1);
	return(0);
}

int BS2C_TypeSmallLongP(BS2CC_CompileContext *ctx, int ty)
{
	if((ty==BS2CC_TYZ_LONG) || (ty==BS2CC_TYZ_ULONG))
		return(1);
	if((ty==BS2CC_TYZ_NLONG) || (ty==BS2CC_TYZ_UNLONG))
		return(1);
	if(BS2C_TypeSmallIntP(ctx, ty))
		return(1);
	return(0);
}

int BS2C_TypeSmallInt128P(BS2CC_CompileContext *ctx, int ty)
{
	if((ty==BS2CC_TYZ_INT128) || (ty==BS2CC_TYZ_UINT128))
		return(1);
	if(BS2C_TypeSmallLongP(ctx, ty))
		return(1);
	return(0);
}

int BS2C_TypeSmallFloatP(BS2CC_CompileContext *ctx, int ty)
{
	if(ty==BS2CC_TYZ_FLOAT)
		return(1);
	if(ty==BS2CC_TYZ_HFLOAT)
		return(1);
	if(BS2C_TypeSmallIntP(ctx, ty))
		return(1);
	return(0);
}

int BS2C_TypeSmallDoubleP(BS2CC_CompileContext *ctx, int ty)
{
	if((ty==BS2CC_TYZ_FLOAT) || (ty==BS2CC_TYZ_DOUBLE))
		return(1);
	if(ty==BS2CC_TYZ_HFLOAT)
		return(1);
	if(BS2C_TypeSmallLongP(ctx, ty))
		return(1);
	return(0);
}

int BS2C_TypeSmallFloat128P(BS2CC_CompileContext *ctx, int ty)
{
	if(ty==BS2CC_TYZ_FLOAT128)
		return(1);
	if((ty==BS2CC_TYZ_FLOAT) || (ty==BS2CC_TYZ_DOUBLE))
		return(1);
	if(ty==BS2CC_TYZ_HFLOAT)
		return(1);
	if(BS2C_TypeSmallInt128P(ctx, ty))
		return(1);
	return(0);
}

int BS2C_TypeSignedP(BS2CC_CompileContext *ctx, int ty)
{
	if(ty==BS2CC_TYZ_INT)
		return(1);
	if(ty==BS2CC_TYZ_SBYTE)
		return(1);
	if(ty==BS2CC_TYZ_SHORT)
		return(1);
	if(ty==BS2CC_TYZ_LONG)
		return(1);
	if(ty==BS2CC_TYZ_NLONG)
		return(1);
	if(ty==BS2CC_TYZ_INT128)
		return(1);
	return(0);
}

int BS2C_TypeUnsignedP(BS2CC_CompileContext *ctx, int ty)
{
	if(ty==BS2CC_TYZ_UINT)
		return(1);
	if(ty==BS2CC_TYZ_UBYTE)
		return(1);
	if(ty==BS2CC_TYZ_USHORT)
		return(1);
	if(ty==BS2CC_TYZ_ULONG)
		return(1);
	if(ty==BS2CC_TYZ_UNLONG)
		return(1);
	if(ty==BS2CC_TYZ_CHAR)
		return(1);
	if(ty==BS2CC_TYZ_CHAR8)
		return(1);
	if(ty==BS2CC_TYZ_BOOL)
		return(1);
	if(ty==BS2CC_TYZ_UINT128)
		return(1);
	return(0);
}

int BS2C_TypeUnsigned2P(BS2CC_CompileContext *ctx, int ty)
{
	if(ty==BS2CC_TYZ_UINT)
		return(1);
	if(ty==BS2CC_TYZ_ULONG)
		return(1);
	if(ty==BS2CC_TYZ_UNLONG)
		return(1);
	if(ty==BS2CC_TYZ_UINT128)
		return(1);
	return(0);
}

int BS2C_TypeAddressP(BS2CC_CompileContext *ctx, int ty)
{
	BS2CC_TypeOverflow *ovf;
	int i;
	
	if((ty&BS2CC_TYT_MASK)==BS2CC_TYT_BASIC)
	{
		if(ty&BS2CC_TYS_MASK)
			return(1);
		if(ty&BS2CC_TYI_MASK)
			return(1);

		i=ty&BS2CC_TYE_MASK;
		if(i>=256)
			return(1);

		if((ty==BS2CC_TYZ_ADDRESS)||
			(ty==BS2CC_TYZ_VARIANT)||
			(ty==BS2CC_TYZ_AUTOVAR)||
			(ty==BS2CC_TYZ_STRING)||
			(ty==BS2CC_TYZ_CSTRING))
				return(1);
		return(0);
	}
	
	if((ty&BS2CC_TYT_MASK)==BS2CC_TYT_OVF)
	{
		/* Almost Certain */
		return(1);
		
//		ovf=ctx->tyovf[ty&BS2CC_TYO_MASK];
//		if(ovf->base>=256)
//			return(1);
//		return(ovf->base==BS2CC_TYZ_ADDRESS);
	}
	
	return(0);
}

int BS2C_TypeNullableP(BS2CC_CompileContext *ctx, int ty)
{
	BS2CC_TypeOverflow *ovf;
	int i;
	
	if((ty&BS2CC_TYT_MASK)==BS2CC_TYT_BASIC)
	{
		if(ty&BS2CC_TYF_NAB)
		{
			if(BS2C_TypeAddressP(ctx, ty))
				return(0);
			return(1);
		}
		return(0);
	}

	if((ty&BS2CC_TYT_MASK)==BS2CC_TYT_OVF)
	{
		ovf=ctx->tyovf[ty&BS2CC_TYO_MASK];
		return(ovf->nab==1);
	}
	
	return(0);
}

int BS2C_TypeNonnullP(BS2CC_CompileContext *ctx, int ty)
{
	BS2CC_TypeOverflow *ovf;
	int i;
	
	if((ty&BS2CC_TYT_MASK)==BS2CC_TYT_BASIC)
	{
		if(ty&BS2CC_TYF_NAB)
		{
			if(BS2C_TypeAddressP(ctx, ty))
				return(1);
			return(0);
		}
		return(0);
	}

	if((ty&BS2CC_TYT_MASK)==BS2CC_TYT_OVF)
	{
		ovf=ctx->tyovf[ty&BS2CC_TYO_MASK];
		return(ovf->nab==1);
	}
	
	return(0);
}

int BS2C_TypeArrayP(BS2CC_CompileContext *ctx, int ty)
{
	BS2CC_TypeOverflow *ovf;

	if((ty&BS2CC_TYT_MASK)==BS2CC_TYT_BASIC)
	{
		if(ty&BS2CC_TYS_MASK)
			return(1);
//		if((ty&BS2CC_TYI_MASK) && ((ty&BS2CC_TYI_MASK)<BS2CC_TYI_P1))
		if((ty&BS2CC_TYI_MASK) && ((ty&BS2CC_TYI_MASK)<BS2CC_TYI_R1))
			return(1);
		return(0);
	}

	if((ty&BS2CC_TYT_MASK)==BS2CC_TYT_OVF)
	{
		ovf=ctx->tyovf[ty&BS2CC_TYO_MASK];
		if(ovf->al)
			return(1);
		if(ovf->an)
			return(1);
		return(0);
	}
	
	return(0);
}

int BS2C_TypeVarRefP(BS2CC_CompileContext *ctx, int ty)
{
	BS2CC_TypeOverflow *ovf;

	if((ty&BS2CC_TYT_MASK)==BS2CC_TYT_BASIC)
	{
//		if(ty&BS2CC_TYS_MASK)
//			return(0);
//		if((ty&BS2CC_TYI_MASK) && ((ty&BS2CC_TYI_MASK)<BS2CC_TYI_P1))
//		if((ty&BS2CC_TYI_MASK) && ((ty&BS2CC_TYI_MASK)<BS2CC_TYI_R1))
		if((ty&BS2CC_TYI_MASK)==BS2CC_TYI_R1)
			return(1);
		return(0);
	}

	if((ty&BS2CC_TYT_MASK)==BS2CC_TYT_OVF)
	{
		ovf=ctx->tyovf[ty&BS2CC_TYO_MASK];
		if(ovf->rf)
			return(1);
		return(0);
	}
	
	return(0);
}

int BS2C_TypePointerP(BS2CC_CompileContext *ctx, int ty)
{
	BS2CC_TypeOverflow *ovf;

	if((ty&BS2CC_TYT_MASK)==BS2CC_TYT_BASIC)
	{
//		if(ty&BS2CC_TYS_MASK)
//			return(1);
//		if((ty&BS2CC_TYI_MASK) && ((ty&BS2CC_TYI_MASK)<BS2CC_TYI_P1))
		if((ty&BS2CC_TYI_MASK) &&
				((ty&BS2CC_TYI_MASK)>=BS2CC_TYI_P1) &&
				((ty&BS2CC_TYI_MASK)<=BS2CC_TYI_P3))
			return(1);
		return(0);
	}

	if((ty&BS2CC_TYT_MASK)==BS2CC_TYT_OVF)
	{
		ovf=ctx->tyovf[ty&BS2CC_TYO_MASK];
//		if(ovf->pl)
		if(ovf->pl && !(ovf->al || ovf->an))
			return(1);
		return(0);
	}
	
	return(0);
}

int BS2C_TypeArrayOrPointerP(BS2CC_CompileContext *ctx, int ty)
{
	if(BS2C_TypeArrayP(ctx, ty))
		return(1);
	if(BS2C_TypePointerP(ctx, ty))
		return(1);
	return(0);
}

int BS2C_TypeFloatP(BS2CC_CompileContext *ctx, int ty)
{
	if(ty==BS2CC_TYZ_FLOAT)
		return(1);
	return(0);
}

int BS2C_TypeDoubleP(BS2CC_CompileContext *ctx, int ty)
{
	if(ty==BS2CC_TYZ_DOUBLE)
		return(1);
	return(0);
}

int BS2C_TypeHalfFloatP(BS2CC_CompileContext *ctx, int ty)
{
	if(ty==BS2CC_TYZ_HFLOAT)
		return(1);
	return(0);
}

int BS2C_TypeInt128P(BS2CC_CompileContext *ctx, int ty)
{
	if(ty==BS2CC_TYZ_INT128)
		return(1);
	return(0);
}

int BS2C_TypeUInt128P(BS2CC_CompileContext *ctx, int ty)
{
	if(ty==BS2CC_TYZ_UINT128)
		return(1);
	return(0);
}

int BS2C_TypeFloat128P(BS2CC_CompileContext *ctx, int ty)
{
	if(ty==BS2CC_TYZ_FLOAT128)
		return(1);
	return(0);
}

// int BS2C_TypeArrayP(BS2CC_CompileContext *ctx, int ty)
//{
//	if(ty&BS2CC_TYS_MASK)
//		return(1);
//	if((ty&BS2CC_TYI_MASK) && ((ty&BS2CC_TYI_MASK)<BS2CC_TYI_P1))
//		return(1);
//	return(0);
//}

int BS2C_TypeSizedArrayP(BS2CC_CompileContext *ctx, int ty)
{
	BS2CC_TypeOverflow *ovf;

	if((ty&BS2CC_TYT_MASK)==BS2CC_TYT_BASIC)
	{
		if(ty&BS2CC_TYS_MASK)
			return(1);
//		if((ty&BS2CC_TYI_MASK) && ((ty&BS2CC_TYI_MASK)<BS2CC_TYI_P1))
//			return(1);
		return(0);
	}

	if((ty&BS2CC_TYT_MASK)==BS2CC_TYT_OVF)
	{
		ovf=ctx->tyovf[ty&BS2CC_TYO_MASK];
		if(ovf->an)
			return(1);
		return(0);
	}
	
	return(0);
}

int BS2C_TypeVoidP(BS2CC_CompileContext *ctx, int ty)
{
	return(ty==BS2CC_TYZ_VOID);
}

int BS2C_TypeVariantP(BS2CC_CompileContext *ctx, int ty)
{
	return(ty==BS2CC_TYZ_VARIANT);
//	return(
//		(ty==BS2CC_TYZ_VARIANT)||
//		(ty==BS2CC_TYZ_AUTOVAR));
}

int BS2C_TypeConcreteP(BS2CC_CompileContext *ctx, int ty)
{
	if(	(ty==BS2CC_TYZ_VARIANT)||
		(ty==BS2CC_TYZ_ADDRESS)||
		(ty==BS2CC_TYZ_AUTOVAR))
			return(0);
	return(1);
}

int BS2C_TypeStringP(BS2CC_CompileContext *ctx, int ty)
{
	return((ty==BS2CC_TYZ_STRING)||
		(ty==BS2CC_TYZ_CSTRING));
}

int BS2C_TypeVec2fP(BS2CC_CompileContext *ctx, int ty)
	{ return(ty==BS2CC_TYZ_VEC2F); }
int BS2C_TypeVec3fP(BS2CC_CompileContext *ctx, int ty)
	{ return(ty==BS2CC_TYZ_VEC3F); }
int BS2C_TypeVec4fP(BS2CC_CompileContext *ctx, int ty)
	{ return(ty==BS2CC_TYZ_VEC4F); }
int BS2C_TypeVec2dP(BS2CC_CompileContext *ctx, int ty)
	{ return(ty==BS2CC_TYZ_VEC2D); }
int BS2C_TypeQuatfP(BS2CC_CompileContext *ctx, int ty)
	{ return(ty==BS2CC_TYZ_QUATF); }

int BS2C_TypeVec3dP(BS2CC_CompileContext *ctx, int ty)
	{ return(ty==BS2CC_TYZ_VEC3D); }
int BS2C_TypeVec4dP(BS2CC_CompileContext *ctx, int ty)
	{ return(ty==BS2CC_TYZ_VEC4D); }
int BS2C_TypeVec3xfP(BS2CC_CompileContext *ctx, int ty)
	{ return(ty==BS2CC_TYZ_VEC3XF); }

int BS2C_TypeFComplexP(BS2CC_CompileContext *ctx, int ty)
	{ return(ty==BS2CC_TYZ_FCPLX); }
int BS2C_TypeDComplexP(BS2CC_CompileContext *ctx, int ty)
	{ return(ty==BS2CC_TYZ_DCPLX); }

int BS2C_TypeSmallFComplexP(BS2CC_CompileContext *ctx, int ty)
{
//	if(BS2C_TypeSmallFloatP(ctx, ty))
	if(BS2C_TypeSmallDoubleP(ctx, ty))
		return(1);
	if(ty==BS2CC_TYZ_FCPLX)
		return(1);
	return(0);
}

int BS2C_TypeSmallDComplexP(BS2CC_CompileContext *ctx, int ty)
{
	if(BS2C_TypeSmallDoubleP(ctx, ty))
		return(1);
	if(ty==BS2CC_TYZ_FCPLX)
		return(1);
	if(ty==BS2CC_TYZ_DCPLX)
		return(1);
	return(0);
}

int BS2C_TypeSmallQuatfP(BS2CC_CompileContext *ctx, int ty)
{
	if(BS2C_TypeSmallFComplexP(ctx, ty))
		return(1);
	if(ty==BS2CC_TYZ_QUATF)
		return(1);
	return(0);
}

int BS2C_TypeNumberP(BS2CC_CompileContext *ctx, int ty)
{
	if(BS2C_TypeSmallDComplexP(ctx, ty))
		return(1);
	if(BS2C_TypeSmallQuatfP(ctx, ty))
		return(1);
	return(0);
}

int BS2C_TypeOpXvCplxP(BS2CC_CompileContext *ctx, int ty)
{
	return(
		(ty==BS2CC_TYZ_QUATF)||
		(ty==BS2CC_TYZ_FCPLX)||
		(ty==BS2CC_TYZ_DCPLX));
}

int BS2C_TypeX64P(BS2CC_CompileContext *ctx, int ty)
{
	return(
		(ty==BS2CC_TYZ_VEC2F)||
		(ty==BS2CC_TYZ_FCPLX));
}

int BS2C_TypeX128P(BS2CC_CompileContext *ctx, int ty)
{
	return(
		(ty==BS2CC_TYZ_VEC3F)||
		(ty==BS2CC_TYZ_VEC4F)||
		(ty==BS2CC_TYZ_VEC2D)||
		(ty==BS2CC_TYZ_QUATF)||
		(ty==BS2CC_TYZ_DCPLX)||
		(ty==BS2CC_TYZ_VEC3XF)||
		(ty==BS2CC_TYZ_INT128)||
		(ty==BS2CC_TYZ_UINT128)||
		(ty==BS2CC_TYZ_FLOAT128));
}

int BS2C_TypeOpXNumP(BS2CC_CompileContext *ctx, int ty)
{
	return(
		(ty==BS2CC_TYZ_INT128)||
		(ty==BS2CC_TYZ_UINT128)||
		(ty==BS2CC_TYZ_FLOAT128));
}

int BS2C_TypeOpXvP(BS2CC_CompileContext *ctx, int ty)
{
	return(
		(ty==BS2CC_TYZ_VEC2F)||
		(ty==BS2CC_TYZ_VEC3F)||
		(ty==BS2CC_TYZ_VEC4F)||
		(ty==BS2CC_TYZ_VEC2D)||
		(ty==BS2CC_TYZ_VEC3XF)||
		(ty==BS2CC_TYZ_QUATF)||
		(ty==BS2CC_TYZ_FCPLX)||
		(ty==BS2CC_TYZ_DCPLX));
}

int BS2C_TypeOpXvFloatP(BS2CC_CompileContext *ctx, int ty)
{
	return(
		(ty==BS2CC_TYZ_VEC2F)||
		(ty==BS2CC_TYZ_VEC3F)||
		(ty==BS2CC_TYZ_VEC4F)||
		(ty==BS2CC_TYZ_QUATF)||
		(ty==BS2CC_TYZ_FCPLX));
}

int BS2C_TypeOpXvDoubleP(BS2CC_CompileContext *ctx, int ty)
{
	return(
		(ty==BS2CC_TYZ_VEC2D)||
		(ty==BS2CC_TYZ_VEC3XF)||
		(ty==BS2CC_TYZ_DCPLX));
}

int BS2C_TypeXvGetElemType(BS2CC_CompileContext *ctx, int ty)
{
	if(	(ty==BS2CC_TYZ_VEC2F)||
		(ty==BS2CC_TYZ_VEC3F)||
		(ty==BS2CC_TYZ_VEC4F)||
		(ty==BS2CC_TYZ_QUATF)||
		(ty==BS2CC_TYZ_FCPLX))
			return(BS2CC_TYZ_FLOAT);

	if(	(ty==BS2CC_TYZ_VEC2D)||
		(ty==BS2CC_TYZ_VEC3XF)||
		(ty==BS2CC_TYZ_DCPLX))
			return(BS2CC_TYZ_DOUBLE);

	return(-1);
}

int BS2C_TypeXvGetElemCount(BS2CC_CompileContext *ctx, int ty)
{
	if(	(ty==BS2CC_TYZ_VEC2F)||
		(ty==BS2CC_TYZ_VEC2D)||
		(ty==BS2CC_TYZ_FCPLX)||
		(ty==BS2CC_TYZ_DCPLX))
			return(2);
	if(	(ty==BS2CC_TYZ_VEC3F)||
		(ty==BS2CC_TYZ_VEC3XF))
			return(3);
	if(	(ty==BS2CC_TYZ_VEC4F)||
		(ty==BS2CC_TYZ_QUATF))
			return(4);
	return(-1);
}


int BS2C_TypeConvIsNarrowingP(BS2CC_CompileContext *ctx,
	int dty, int sty)
{
	/* Narrowing concept only applies to numeric types. */
	if(!BS2C_TypeNumberP(ctx, dty) ||
		!BS2C_TypeNumberP(ctx, sty))
			return(0);

	if(dty==sty)
		return(0);

	if(dty==BS2CC_TYZ_UBYTE)
	{
		if(sty==BS2CC_TYZ_BOOL)
			return(0);
		return(1);
	}
	if(dty==BS2CC_TYZ_SBYTE)
	{
		if(sty==BS2CC_TYZ_BOOL)
			return(0);
		return(1);
	}

	if(dty==BS2CC_TYZ_USHORT)
	{
		if(sty==BS2CC_TYZ_UBYTE)
			return(0);
		if(sty==BS2CC_TYZ_CHAR8)
			return(0);
		if(sty==BS2CC_TYZ_CHAR)
			return(0);
		if(sty==BS2CC_TYZ_BOOL)
			return(0);
		return(1);
	}

	if(dty==BS2CC_TYZ_SHORT)
	{
		if(sty==BS2CC_TYZ_UBYTE)
			return(0);
		if(sty==BS2CC_TYZ_SBYTE)
			return(0);
		if(sty==BS2CC_TYZ_CHAR8)
			return(0);
		if(sty==BS2CC_TYZ_BOOL)
			return(0);
		return(1);
	}

	if(dty==BS2CC_TYZ_UINT)
	{
		if(	(sty==BS2CC_TYZ_UBYTE) ||
			(sty==BS2CC_TYZ_USHORT) ||
			(sty==BS2CC_TYZ_CHAR8) ||
			(sty==BS2CC_TYZ_CHAR) ||
			(sty==BS2CC_TYZ_BOOL))
				return(0);
		return(1);
	}

	if(dty==BS2CC_TYZ_INT)
	{
		if(	(sty==BS2CC_TYZ_UBYTE) ||
			(sty==BS2CC_TYZ_SBYTE) ||
			(sty==BS2CC_TYZ_USHORT) ||
			(sty==BS2CC_TYZ_SHORT) ||
			(sty==BS2CC_TYZ_CHAR8) ||
			(sty==BS2CC_TYZ_CHAR) ||
			(sty==BS2CC_TYZ_BOOL))
				return(0);
		return(1);
	}

	if(dty==BS2CC_TYZ_ULONG)
	{
		if(	(sty==BS2CC_TYZ_UBYTE) ||
			(sty==BS2CC_TYZ_USHORT) ||
			(sty==BS2CC_TYZ_UINT) ||
			(sty==BS2CC_TYZ_CHAR8) ||
			(sty==BS2CC_TYZ_CHAR) ||
			(sty==BS2CC_TYZ_BOOL))
				return(0);
		return(1);
	}

	if(dty==BS2CC_TYZ_LONG)
	{
		if(	(sty==BS2CC_TYZ_UBYTE) ||
			(sty==BS2CC_TYZ_SBYTE) ||
			(sty==BS2CC_TYZ_USHORT) ||
			(sty==BS2CC_TYZ_SHORT) ||
			(sty==BS2CC_TYZ_UINT) ||
			(sty==BS2CC_TYZ_INT) ||
			(sty==BS2CC_TYZ_CHAR8) ||
			(sty==BS2CC_TYZ_CHAR) ||
			(sty==BS2CC_TYZ_BOOL))
				return(0);
		return(1);
	}

	if(dty==BS2CC_TYZ_UINT128)
	{
		if(	(sty==BS2CC_TYZ_UBYTE) ||
			(sty==BS2CC_TYZ_USHORT) ||
			(sty==BS2CC_TYZ_UINT) ||
			(sty==BS2CC_TYZ_ULONG) ||
			(sty==BS2CC_TYZ_CHAR8) ||
			(sty==BS2CC_TYZ_CHAR) ||
			(sty==BS2CC_TYZ_BOOL))
				return(0);
		return(1);
	}

	if(dty==BS2CC_TYZ_INT128)
	{
		if(	(sty==BS2CC_TYZ_UBYTE) ||
			(sty==BS2CC_TYZ_SBYTE) ||
			(sty==BS2CC_TYZ_USHORT) ||
			(sty==BS2CC_TYZ_SHORT) ||
			(sty==BS2CC_TYZ_UINT) ||
			(sty==BS2CC_TYZ_INT) ||
			(sty==BS2CC_TYZ_ULONG) ||
			(sty==BS2CC_TYZ_LONG) ||
			(sty==BS2CC_TYZ_CHAR8) ||
			(sty==BS2CC_TYZ_CHAR) ||
			(sty==BS2CC_TYZ_BOOL))
				return(0);
		return(1);
	}

	if(dty==BS2CC_TYZ_HFLOAT)
	{
		if(	(sty==BS2CC_TYZ_UBYTE) ||
			(sty==BS2CC_TYZ_SBYTE) ||
			(sty==BS2CC_TYZ_BOOL))
				return(0);
		return(1);
	}

	if(dty==BS2CC_TYZ_FLOAT)
	{
		if(	(sty==BS2CC_TYZ_UBYTE) ||
			(sty==BS2CC_TYZ_SBYTE) ||
			(sty==BS2CC_TYZ_USHORT) ||
			(sty==BS2CC_TYZ_SHORT) ||
			(sty==BS2CC_TYZ_CHAR8) ||
			(sty==BS2CC_TYZ_CHAR) ||
			(sty==BS2CC_TYZ_BOOL))
				return(0);
		return(1);
	}

	if(dty==BS2CC_TYZ_DOUBLE)
	{
		if(	(sty==BS2CC_TYZ_UBYTE) ||
			(sty==BS2CC_TYZ_SBYTE) ||
			(sty==BS2CC_TYZ_USHORT) ||
			(sty==BS2CC_TYZ_SHORT) ||
			(sty==BS2CC_TYZ_UINT) ||
			(sty==BS2CC_TYZ_INT) ||
			(sty==BS2CC_TYZ_FLOAT) ||
			(sty==BS2CC_TYZ_CHAR8) ||
			(sty==BS2CC_TYZ_CHAR) ||
			(sty==BS2CC_TYZ_BOOL))
				return(0);
		return(1);
	}

	return(0);
}


int BS2C_TypeDerefType(BS2CC_CompileContext *ctx, int ty)
{
	BS2CC_TypeOverflow tovf;
	BS2CC_TypeOverflow *ovf;

	int ty1;
	int al;
	int i;

	if((ty&BS2CC_TYT_MASK)==BS2CC_TYT_BASIC)
	{
		if(ty&BS2CC_TYS_MASK)
		{
			ty1=ty&(BS2CC_TYE_MASK|BS2CC_TYI_MASK);
			return(ty1);
		}
		
		al=(ty>>BS2CC_TYI_SHR)&BS2CC_TYI_MASK2;

		if(!al)
		{
			if(ty==BS2CC_TYZ_VARIANT)
				return(ty);
			if(ty==BS2CC_TYZ_AUTOVAR)
				return(ty);

			BS2C_CompileError(ctx, BS2CC_ERRN_ERRCANTDEREF);
			return(ty);
		}
		
//		if((al==1) || (al==5))
		if((al==1) || (al==5) || (al==4))
		{
			ty1=ty&BS2CC_TYE_MASK;
			return(ty1);
		}
		
		ty1=ty-BS2CC_TYI_A1;
		return(ty1);
	}

	if((ty&BS2CC_TYT_MASK)==BS2CC_TYT_OVF)
	{
//		memset(&tovf, 0, sizeof(BS2CC_TypeOverflow));
		ovf=ctx->tyovf[ty&BS2CC_TYO_MASK];
		memcpy(&tovf, ovf, sizeof(BS2CC_TypeOverflow));

		if(ovf->rf)
		{
			tovf.rf=ovf->rf-1;
			ty=BS2C_TypeFromTypeOverflow(ctx, &tovf);
			return(ty);
		}

		if(ovf->an)
		{
			for(i=0; i<ovf->an; i++)
				tovf.asz[i]=tovf.asz[i+1];
			tovf.an=ovf->an-1;
			ty=BS2C_TypeFromTypeOverflow(ctx, &tovf);
			return(ty);
		}
		
		if(ovf->al)
		{
			tovf.al=ovf->al-1;
			ty=BS2C_TypeFromTypeOverflow(ctx, &tovf);
			return(ty);
		}

		if(ovf->pl)
		{
			tovf.pl=ovf->pl-1;
			ty=BS2C_TypeFromTypeOverflow(ctx, &tovf);
			return(ty);
		}

		return(0);
	}
	
	BS2C_CaseError(ctx);
	return(-1);
}

int BS2C_TypeRefType(BS2CC_CompileContext *ctx, int ty)
{
	BS2CC_TypeOverflow *ovf;

	int ty1;

	if((ty&BS2CC_TYT_MASK)==BS2CC_TYT_BASIC)
	{
		if(ty&BS2CC_TYS_MASK)
		{
			BS2C_CaseError(ctx);
			return(-1);
		}

		if(ty&BS2CC_TYI_MASK)
		{
			BS2C_CaseError(ctx);
			return(-1);
		}
		
		ty1=ty|BS2CC_TYI_R1;
		return(ty1);
	}

	BS2C_CaseError(ctx);
	return(-1);
}

int BS2C_TypeGetArraySize(BS2CC_CompileContext *ctx, int ty)
{
	BS2CC_TypeOverflow *ovf;
	int ty1, sz;
	int al;

	if((ty&BS2CC_TYT_MASK)==BS2CC_TYT_BASIC)
	{
		if(ty&BS2CC_TYS_MASK)
		{
			sz=(ty>>BS2CC_TYS_SHR)&BS2CC_TYS_MASK2;
			return(sz);
		}
		
		al=(ty>>BS2CC_TYI_SHR)&BS2CC_TYI_MASK2;
		if(!al)
		{
			BS2C_CompileError(ctx, BS2CC_ERRN_ERRCANTDEREF);
			return(0);
		}
		return(0);
	}

	if((ty&BS2CC_TYT_MASK)==BS2CC_TYT_OVF)
	{
		ovf=ctx->tyovf[ty&BS2CC_TYO_MASK];
		
		if(ovf->an>=1)
		{
			return(ovf->asz[0]);
		}
	}

	BS2C_CaseError(ctx);
	return(0);
}

int BS2C_InferSuperType(
	BS2CC_CompileContext *ctx, int lty, int rty)
{
	if(lty==rty)
		return(lty);
	
	if(BS2C_TypeSmallIntP(ctx, lty) &&
		BS2C_TypeSmallIntP(ctx, rty))
	{
		return(BS2CC_TYZ_INT);
	}

	if(BS2C_TypeSmallLongP(ctx, lty) &&
		BS2C_TypeSmallLongP(ctx, rty))
	{
		return(BS2CC_TYZ_LONG);
	}

	if(BS2C_TypeSmallInt128P(ctx, lty) &&
		BS2C_TypeSmallInt128P(ctx, rty))
	{
		return(BS2CC_TYZ_INT128);
	}

	if(BS2C_TypeSmallFloatP(ctx, lty) &&
		BS2C_TypeSmallFloatP(ctx, rty))
	{
		return(BS2CC_TYZ_FLOAT);
	}

	if(BS2C_TypeSmallDoubleP(ctx, lty) &&
		BS2C_TypeSmallDoubleP(ctx, rty))
	{
		return(BS2CC_TYZ_DOUBLE);
	}

	if(BS2C_TypeSmallFloat128P(ctx, lty) &&
		BS2C_TypeSmallFloat128P(ctx, rty))
	{
		return(BS2CC_TYZ_FLOAT128);
	}

	if(BS2C_TypeSmallFComplexP(ctx, lty) &&
		BS2C_TypeSmallFComplexP(ctx, rty))
	{
		return(BS2CC_TYZ_FCPLX);
	}

	if(BS2C_TypeSmallDComplexP(ctx, lty) &&
		BS2C_TypeSmallDComplexP(ctx, rty))
	{
		return(BS2CC_TYZ_DCPLX);
	}

	if(BS2C_TypeSmallQuatfP(ctx, lty) &&
		BS2C_TypeSmallQuatfP(ctx, rty))
	{
		return(BS2CC_TYZ_QUATF);
	}

	if((lty==BS2CC_TYZ_VEC2F) ||
		(lty==BS2CC_TYZ_VEC2D))
	{
		if((rty==BS2CC_TYZ_VEC2F) ||
			(rty==BS2CC_TYZ_VEC2D))
				return(BS2CC_TYZ_VEC2D);
	}

	if((lty==BS2CC_TYZ_VEC3F) ||
		(lty==BS2CC_TYZ_VEC3XF))
	{
		if((rty==BS2CC_TYZ_VEC3F) ||
			(rty==BS2CC_TYZ_VEC3XF))
				return(BS2CC_TYZ_VEC3XF);
	}

	if((lty==BS2CC_TYZ_VEC2F) ||
		(lty==BS2CC_TYZ_VEC2D) ||
		(lty==BS2CC_TYZ_VEC3F) ||
		(lty==BS2CC_TYZ_VEC3XF))
	{
		if(	(rty==BS2CC_TYZ_VEC2F) ||
			(rty==BS2CC_TYZ_VEC2D) ||
			(rty==BS2CC_TYZ_VEC3F) ||
			(rty==BS2CC_TYZ_VEC3XF))
				return(BS2CC_TYZ_VEC3XF);
	}
	
	if(	(lty==BS2CC_TYZ_VEC4F) ||
		(lty==BS2CC_TYZ_QUATF))
	{
		if(	(rty==BS2CC_TYZ_VEC2F) ||
			(rty==BS2CC_TYZ_VEC2D) ||
			(rty==BS2CC_TYZ_VEC3F) ||
			(rty==BS2CC_TYZ_VEC3XF))
				return(lty);
	}

	if(	(rty==BS2CC_TYZ_VEC4F) ||
		(rty==BS2CC_TYZ_QUATF))
	{
		if(	(lty==BS2CC_TYZ_VEC2F) ||
			(lty==BS2CC_TYZ_VEC2D) ||
			(lty==BS2CC_TYZ_VEC3F) ||
			(lty==BS2CC_TYZ_VEC3XF))
				return(rty);
	}

	if((lty==BS2CC_TYZ_AUTOVAR) && (rty!=BS2CC_TYZ_AUTOVAR))
		return(rty);
	if((lty!=BS2CC_TYZ_AUTOVAR) && (rty==BS2CC_TYZ_AUTOVAR))
		return(lty);

	if(BS2C_TypeVariantP(ctx, lty) ||
		BS2C_TypeVariantP(ctx, rty))
	{
		return(BS2CC_TYZ_VARIANT);
	}

	BS2C_CaseError(ctx);
	return(BS2CC_TYZ_VARIANT);
}

int BS2C_TypeAssignSuperType(
	BS2CC_CompileContext *ctx, int lty, int rty)
{
	int lty1, rty1;

	if(BS2C_TypeArrayP(ctx, lty) && BS2C_TypeArrayP(ctx, rty))
	{
		lty1=BS2C_TypeDerefType(ctx, lty);
		rty1=BS2C_TypeDerefType(ctx, rty);
		
		if(BS2C_TypeCompatibleP(ctx, lty1, rty1))
		{
			return(lty);
		}

		if(BS2C_TypeAddressP(ctx, lty1) &&
			BS2C_TypeAddressP(ctx, rty1))
		{
			return(lty);
		}
		
		BS2C_CaseError(ctx);
		return(lty);
	}

	if(BS2C_TypeSmallIntP(ctx, lty) &&
		BS2C_TypeSmallIntP(ctx, rty))
	{
		return(lty);
	}
	
	if(BS2C_TypeCompatibleP(ctx, lty, rty))
		return(lty);

	if(BS2C_TypeAddressP(ctx, lty) &&
		(rty==BS2CC_TYZ_ADDRESS))
			return(lty);

	if(BS2C_TypePointerP(ctx, lty) &&
		BS2C_TypeAddressP(ctx, rty))
			return(lty);

	if((lty==BS2CC_TYZ_VEC2F) ||
		(lty==BS2CC_TYZ_VEC2D))
	{
		if((rty==BS2CC_TYZ_VEC2F) ||
			(rty==BS2CC_TYZ_VEC2D))
				return(lty);
	}

	if((lty==BS2CC_TYZ_VEC3F) ||
		(lty==BS2CC_TYZ_VEC3XF))
	{
		if((rty==BS2CC_TYZ_VEC3F) ||
			(rty==BS2CC_TYZ_VEC3XF))
				return(lty);
	}

	lty1=BS2C_InferSuperType(ctx, lty, rty);
	return(lty1);
}

int BS2C_TypeBinarySuperType(
	BS2CC_CompileContext *ctx, char *op, int lty, int rty)
{
	int lty1, rty1;

	if(BS2C_TypeArrayP(ctx, lty) &&
		BS2C_TypeSmallIntP(ctx, rty) &&
		(!strcmp(op, "+") || !strcmp(op, "-")))
	{
		return(lty);
	}

	if(BS2C_TypePointerP(ctx, lty) &&
		BS2C_TypeSmallIntP(ctx, rty) &&
		(!strcmp(op, "+") || !strcmp(op, "-")))
	{
		return(lty);
	}

	if(BS2C_TypeArrayP(ctx, lty) &&
		BS2C_TypeArrayP(ctx, rty) &&
		!strcmp(op, "-"))
	{
		return(BS2CC_TYZ_INT);
	}

	if(BS2C_TypePointerP(ctx, lty) &&
		BS2C_TypePointerP(ctx, rty) &&
		!strcmp(op, "-"))
	{
		return(BS2CC_TYZ_INT);
	}

	if(BS2C_TypeStringP(ctx, lty) &&
		(!strcmp(op, "+") || !strcmp(op, "-") ||
		!strcmp(op, "&")))
	{
		return(lty);
	}
	
	if(!strcmp(op, "&&") || !strcmp(op, "||"))
	{
		return(BS2CC_TYZ_INT);
	}

	if(BS2C_TypeSmallInt128P(ctx, lty) &&
		BS2C_TypeSmallIntP(ctx, rty) &&
			(!strcmp(op, "<<") ||
			 !strcmp(op, ">>") ||
			 !strcmp(op, ">>>")))
	{
		return(lty);
	}

#if 0
	if(BS2C_TypeArrayP(ctx, lty) && BS2C_TypeArrayP(ctx, rty))
	{
		lty1=BS2C_TypeDerefType(ctx, lty);
		rty1=BS2C_TypeDerefType(ctx, rty);
		
		if(BS2C_TypeCompatibleP(ctx, lty1, rty1))
		{
			return(lty);
		}
	}
#endif

	if(BS2C_TypeOpXvP(ctx, lty) &&
		BS2C_TypeSmallDoubleP(ctx, rty) &&
		(!strcmp(op, "*") || !strcmp(op, "/")))
	{
		return(lty);
	}

	if(	BS2C_TypeSmallDoubleP(ctx, lty) &&
		BS2C_TypeOpXvP(ctx, rty) &&
		(!strcmp(op, "*") || !strcmp(op, "/")))
	{
		return(rty);
	}

	lty1=BS2C_InferSuperType(ctx, lty, rty);
	
	/* Can't do arithmetic directly on half-floats */
	if(BS2C_TypeHalfFloatP(ctx, lty1))
		{ lty1=BS2CC_TYZ_FLOAT; }
	
	return(lty1);
}

int BS2C_TypeCompareSuperType(
	BS2CC_CompileContext *ctx, int lty, int rty)
{
	int lty1, rty1;

	if(BS2C_TypeCompatibleP(ctx, lty, rty))
		{ return(lty); }
	if(BS2C_TypeCompatibleP(ctx, rty, lty))
		{ return(rty); }

	lty1=BS2C_InferSuperType(ctx, lty, rty);
	
	/* Can't do arithmetic directly on half-floats */
	if(BS2C_TypeHalfFloatP(ctx, lty1))
		{ lty1=BS2CC_TYZ_FLOAT; }
	
	return(lty1);
}

int BS2C_TypeCompatibleP(
	BS2CC_CompileContext *ctx, int dty, int sty)
{
	BS2CC_TypeOverflow *ovf;
	BS2CC_VarInfo *lvi, *rvi, *rvi2;
	int lbt, rbt, lal, ral, lsz, rsz;
	int i, j, k, l;

	if(dty==sty)
		return(1);

	if((dty<0) || (sty<0))
		return(0);

	if((dty&BS2CC_TYT_MASK)==BS2CC_TYT_BASIC)
	{
		lbt=dty&BS2CC_TYE_MASK;
		lal=(dty>>BS2CC_TYI_SHR)&BS2CC_TYI_MASK2;
		lsz=(dty>>BS2CC_TYS_SHR)&BS2CC_TYS_MASK2;
	}else if((dty&BS2CC_TYT_MASK)==BS2CC_TYT_OVF)
	{
		ovf=ctx->tyovf[dty&BS2CC_TYO_MASK];
		lbt=ovf->base;
		lal=ovf->al;
		lsz=ovf->asz[0];
	}

	if((sty&BS2CC_TYT_MASK)==BS2CC_TYT_BASIC)
	{
		rbt=sty&BS2CC_TYE_MASK;
		ral=(sty>>BS2CC_TYI_SHR)&BS2CC_TYI_MASK2;
		rsz=(sty>>BS2CC_TYS_SHR)&BS2CC_TYS_MASK2;
	}else if((sty&BS2CC_TYT_MASK)==BS2CC_TYT_OVF)
	{
		ovf=ctx->tyovf[sty&BS2CC_TYO_MASK];
		rbt=ovf->base;
		ral=ovf->al;
		rsz=ovf->asz[0];
	}
	
//	if((lal==1) || (lal==5))
	if(lal==1)
	{
		if((ral!=1) && !rsz)
			return(0);

//		if((ral!=1) && (ral!=5) && !rsz)
//			return(0);
	}else if(lal!=0)
	{
		if(lal!=ral)
			return(0);
	}else
	{
		if(ral || rsz)
			return(0);
	}
	
	if((lbt>=256) && (rbt>=256))
	{
		if(lbt==rbt)
			return(1);
	
		lvi=ctx->globals[lbt-256];
		rvi=ctx->globals[rbt-256];
		
		if((lvi->vitype==BS2CC_VITYPE_GBLFUNC) &&
			(rvi->vitype==BS2CC_VITYPE_GBLFUNC))
		{
			if(!BS2C_TypeCompatibleP(ctx, lvi->rty, rvi->rty))
				return(0);
			if(lvi->nargs!=rvi->nargs)
				return(0);
			l=lvi->nargs;
			for(i=0; i<l; i++)
			{
				if(!BS2C_TypeCompatibleP(ctx,
						lvi->args[i]->bty, rvi->args[i]->bty))
					return(0);
			}
			return(1);
		}

		if((lvi->vitype==BS2CC_VITYPE_CLASS) &&
			(rvi->vitype==BS2CC_VITYPE_CLASS))
		{
			rvi2=rvi;
			while(rvi2)
			{
				if(rvi2==lvi)
					break;
				rvi2=rvi2->super;
			}
			if(rvi2)
				return(1);

			return(0);
		}

		if((lvi->vitype==BS2CC_VITYPE_IFACE) &&
			(rvi->vitype==BS2CC_VITYPE_CLASS))
		{
			rvi2=rvi;
			while(rvi2)
			{
				if(rvi2==lvi)
					break;
				for(i=0; i<rvi2->niface; i++)
					if(rvi2->iface[i]==lvi)
						break;
				if(i<rvi2->niface)
					break;
				rvi2=rvi2->super;
			}
			if(rvi2)
				return(1);

			return(0);
		}

		return(0);
	}

	if(lbt==rbt)
		return(1);

	if((lbt==BS2CC_TYZ_CSTRING) && (rbt==BS2CC_TYZ_STRING))
		return(1);
	if((lbt==BS2CC_TYZ_STRING) && (rbt==BS2CC_TYZ_CSTRING))
		return(1);

	return(0);
}

int BS2C_TypeBaseType(BS2CC_CompileContext *ctx, dtVal expr)
{
	dtVal arrs;
	char *tyn;
	int i;

	arrs=BS2P_GetAstNodeAttr(expr, "arrays");
	if(dtvTrueP(arrs))
		return(BS2CC_TYZ_ADDRESS);

	i=BS2P_GetAstNodeAttrI(expr, "ptrlvl");
	if(i)
		return(BS2CC_TYZ_ADDRESS);

	i=BS2P_GetAstNodeAttrI(expr, "reflvl");
	if(i)
		return(BS2CC_TYZ_ADDRESS);

	tyn=BS2P_GetAstNodeAttrS(expr, "name");
	
	if(tyn)
	{
		if(!strcmp(tyn, "int"))
			return(BS2CC_TYZ_INT);
		if(!strcmp(tyn, "uint"))
			return(BS2CC_TYZ_UINT);
		if(!strcmp(tyn, "long"))
			return(BS2CC_TYZ_LONG);
		if(!strcmp(tyn, "ulong"))
			return(BS2CC_TYZ_ULONG);
		if(!strcmp(tyn, "float"))
			return(BS2CC_TYZ_FLOAT);
		if(!strcmp(tyn, "double"))
			return(BS2CC_TYZ_DOUBLE);
		if(!strcmp(tyn, "byte"))
			return(BS2CC_TYZ_UBYTE);
		if(!strcmp(tyn, "sbyte"))
			return(BS2CC_TYZ_SBYTE);
		if(!strcmp(tyn, "short"))
			return(BS2CC_TYZ_SHORT);
		if(!strcmp(tyn, "ushort"))
			return(BS2CC_TYZ_USHORT);
		if(!strcmp(tyn, "nlong"))
			return(BS2CC_TYZ_NLONG);
		if(!strcmp(tyn, "unlong"))
			return(BS2CC_TYZ_UNLONG);
		if(!strcmp(tyn, "var"))
			return(BS2CC_TYZ_VARIANT);
		if(!strcmp(tyn, "variant"))
			return(BS2CC_TYZ_VARIANT);
		if(!strcmp(tyn, "auto"))
			return(BS2CC_TYZ_AUTOVAR);
		if(!strcmp(tyn, "void"))
			return(BS2CC_TYZ_VOID);
		if(!strcmp(tyn, "string"))
			return(BS2CC_TYZ_STRING);
		if(!strcmp(tyn, "cstring"))
			return(BS2CC_TYZ_CSTRING);
		if(!strcmp(tyn, "char"))
			return(BS2CC_TYZ_CHAR);
		if(!strcmp(tyn, "char8"))
			return(BS2CC_TYZ_CHAR8);
		if(!strcmp(tyn, "bool"))
			return(BS2CC_TYZ_BOOL);
		if(!strcmp(tyn, "hfloat"))
			return(BS2CC_TYZ_HFLOAT);

		if(!strcmp(tyn, "symbol"))
			return(BS2CC_TYZ_SYMBOL);
		if(!strcmp(tyn, "keyword"))
			return(BS2CC_TYZ_KEYWORD);

		if(!strcmp(tyn, "int128"))
			return(BS2CC_TYZ_INT128);
		if(!strcmp(tyn, "uint128"))
			return(BS2CC_TYZ_UINT128);
		if(!strcmp(tyn, "float128"))
			return(BS2CC_TYZ_FLOAT128);

		if(!strcmp(tyn, "vec2f"))
			return(BS2CC_TYZ_VEC2F);
		if(!strcmp(tyn, "vec3f"))
			return(BS2CC_TYZ_VEC3F);
		if(!strcmp(tyn, "vec4f"))
			return(BS2CC_TYZ_VEC4F);
		if(!strcmp(tyn, "vec2d"))
			return(BS2CC_TYZ_VEC2D);
		if(!strcmp(tyn, "quatf"))
			return(BS2CC_TYZ_QUATF);
		if(!strcmp(tyn, "fcomplex"))
			return(BS2CC_TYZ_FCPLX);
		if(!strcmp(tyn, "dcomplex"))
			return(BS2CC_TYZ_DCPLX);

		if(!strcmp(tyn, "vec3d"))
			return(BS2CC_TYZ_VEC3D);
		if(!strcmp(tyn, "vec4d"))
			return(BS2CC_TYZ_VEC4D);
		if(!strcmp(tyn, "vec3xf"))
			return(BS2CC_TYZ_VEC3XF);

		if(!strcmp(tyn, "vec2"))
			return(BS2CC_TYZ_VEC2D);
		if(!strcmp(tyn, "vec3"))
			return(BS2CC_TYZ_VEC3XF);
		if(!strcmp(tyn, "vec4"))
			return(BS2CC_TYZ_VEC4F);
		if(!strcmp(tyn, "quat"))
			return(BS2CC_TYZ_QUATF);
	}
	return(BS2CC_TYZ_ADDRESS);
}

int BS2C_TypeExtType(BS2CC_CompileContext *ctx, dtVal expr)
{
	int cty;

	if(!ctx->frm || !ctx->frm->func)
	{
		cty=BS2C_TypeBaseType(ctx, expr);
		return(cty);
	}
	
	cty=BS2C_TypeRefinedType(ctx, ctx->frm->func, expr);
	return(cty);
}

int BS2C_TypeExtTypeNew(BS2CC_CompileContext *ctx, dtVal expr)
{
	int cty;

	if(!ctx->frm || !ctx->frm->func)
	{
		cty=BS2C_TypeBaseType(ctx, expr);
		return(cty);
	}
	
	cty=BS2C_TypeRefinedType2(ctx, ctx->frm->func, expr, 1);
	return(cty);
}

int BS2C_TypeLookupTypeOverflow(
	BS2CC_CompileContext *ctx, BS2CC_TypeOverflow *tovf)
{
	BS2CC_TypeOverflow *tovf1;
	int i;
	
	for(i=0; i<ctx->ntyovf; i++)
	{
		tovf1=ctx->tyovf[i];
		if(!memcmp(tovf1, tovf, sizeof(BS2CC_TypeOverflow)))
			return(i);
	}
	return(-1);
}

int BS2C_TypeGetTypeOverflow(
	BS2CC_CompileContext *ctx, BS2CC_TypeOverflow *tovf)
{
	BS2CC_TypeOverflow *tovf1;
	int i;

	i=BS2C_TypeLookupTypeOverflow(ctx, tovf);
	if(i>=0)
		return(i);
	
	if(ctx->ntyovf<4096)
	{
		tovf1=dtmAlloc("bs2cc_typeoverflow_t",
			sizeof(BS2CC_TypeOverflow));
		memcpy(tovf1, tovf, sizeof(BS2CC_TypeOverflow));
	
		i=ctx->ntyovf++;
		ctx->tyovf[i]=tovf1;
		return(i);
	}
	
	return(-1);
}

int BS2C_TypeFromTypeOverflow(
	BS2CC_CompileContext *ctx, BS2CC_TypeOverflow *ovf)
{
	int ty, bt, al, al2, asz, nab;
	int i;

	bt=ovf->base;
	asz=ovf->asz[0];

	al2=ovf->al;
	nab=ovf->nab;

	if(ovf->an)
	{
		if(ovf->an>=2)
			{ al2=-1; }
		else
			{ al2=0; }
	}
	if(ovf->pl)
	{
		if(!al2 && (ovf->pl>=1) && (ovf->pl<=3))
			{ al2=5+ovf->pl-1; }
		else
			{ al2=-1; }
	}
	if(ovf->rf)
	{
		if(!al2 && (ovf->rf==1))
			{ al2=4; }
		else
			{ al2=-1; }
	}

	if((bt>=0x0000) && (bt<=BS2CC_TYE_MASK2) &&
		(al2>=0) && (al2<=7) &&
		(asz>=0) && (asz<=BS2CC_TYS_MASK2) &&
		!((ovf->pl>0) && (asz>0)))
	{
		ty=(bt<<BS2CC_TYE_SHR)|(al2<<BS2CC_TYI_SHR)|
			(asz<<BS2CC_TYS_SHR);
		if(nab)
			ty|=BS2CC_TYF_NAB;
		return(ty);
	}

	i=BS2C_TypeGetTypeOverflow(ctx, ovf);
	if(i>=0)
	{
		ty=BS2CC_TYT_OVF|i;
		return(ty);
	}

	BS2C_CaseError(ctx);
	return(BS2CC_TYZ_VARIANT);
}

int BS2C_TypeRefinedType(
	BS2CC_CompileContext *ctx,
	BS2CC_VarInfo *vari,
	dtVal expr)
{
	return(BS2C_TypeRefinedType2(ctx, vari, expr, 0));
}

int BS2C_TypeRefinedType2(
	BS2CC_CompileContext *ctx,
	BS2CC_VarInfo *vari,
	dtVal expr, int flag)
{
	BS2CC_TypeOverflow tovf;
	int asza[16];
	BS2CC_VarInfo *vi;
	dtVal arrs;
	dtVal n0, n1;
	char *tyn;
	char *tag;
	int al, an, al2, pl, rl, asz, bt, ty, nab;
	int i, j, k, l;

	arrs=BS2P_GetAstNodeAttr(expr, "arrays");
//	if(dtvTrueP(arrs))
//		return(BS2CC_TYZ_ADDRESS);

	if(dtvIsArrayP(arrs))
	{
		l=dtvArrayGetSize(arrs); an=0;
		for(i=0; i<l; i++)
		{
			n0=dtvArrayGetIndexDtVal(arrs, i);

			tag=BS2P_GetAstNodeTag(n0);
		
			if(tag && !strcmp(tag, "arrdef"))
			{
				n1=BS2P_GetAstNodeAttr(n0, "value");
				n1=BS2C_ReduceExpr(ctx, n1);
				if(dtvIsSmallIntP(n1))
				{
					j=dtvUnwrapInt(n1);
					asza[i]=j;
				}else
				{
					if(!(flag&1))
						BS2C_CompileError(ctx, BS2CC_ERRN_ERRSZNOTCONST);
					asza[i]=0;
				}
//				asza[i]=n1;
				an=i+1;
			}
			else
				{ asza[i]=0; }
			if(ctx->ncfatal)
				break;
		}

		asz=0;
		al=l;
		an=(an>0)?l:0;
	}else if(dtvTrueP(arrs))
	{
		tag=BS2P_GetAstNodeTag(arrs);
		
		if(tag && !strcmp(tag, "arrdef"))
		{
			al=0;
//			asz=BS2P_GetAstNodeAttrI(arrs, "value");

			n1=BS2P_GetAstNodeAttr(arrs, "value");
			n1=BS2C_ReduceExpr(ctx, n1);
			if(dtvIsSmallIntP(n1))
			{
				al=0; asz=dtvUnwrapInt(n1);
				asza[0]=asz; an=1;
			}else
			{
				if(!(flag&1))
					BS2C_CompileError(ctx, BS2CC_ERRN_ERRSZNOTCONST);
				al=1; asz=0; an=0;
			}
		}else
		{
			al=1; asz=0; an=0;
		}
	}else
	{
		al=0; asz=0; an=0;
	}

	pl=BS2P_GetAstNodeAttrI(expr, "ptrlvl");
	rl=BS2P_GetAstNodeAttrI(expr, "reflvl");
//	if(pl)
//		return(BS2CC_TYZ_ADDRESS);

	nab=BS2P_GetAstNodeAttrI(expr, "nullable");

	al2=al;
	if(an>=2)al2=-1;
	if(pl)
	{
		if(!al2 && (pl>=1) && (pl<=3))
			{ al2=5+pl-1; }
		else
			{ al2=-1; }
	}
	if(rl)
	{
		if(!al2 && (rl==1))
			{ al2=4; }
		else
			{ al2=-1; }
	}

//	if(pl)al=5+pl;
//	if(rl)al=4;

	tyn=BS2P_GetAstNodeAttrS(expr, "name");
	
	if(tyn)
	{
		if(!strcmp(tyn, "void"))
			{ bt=BS2CC_TYZ_VOID; }
		else if(!strcmp(tyn, "int"))
			{ bt=BS2CC_TYZ_INT; }
		else if(!strcmp(tyn, "uint"))
			{ bt=BS2CC_TYZ_UINT; }
		else if(!strcmp(tyn, "long"))
			{ bt=BS2CC_TYZ_LONG; }
		else if(!strcmp(tyn, "ulong"))
			{ bt=BS2CC_TYZ_ULONG; }
		else if(!strcmp(tyn, "float"))
			{ bt=BS2CC_TYZ_FLOAT; }
		else if(!strcmp(tyn, "double"))
			{ bt=BS2CC_TYZ_DOUBLE; }
		else if(!strcmp(tyn, "byte"))
			{ bt=BS2CC_TYZ_UBYTE; }
		else if(!strcmp(tyn, "sbyte"))
			{ bt=BS2CC_TYZ_SBYTE; }
		else if(!strcmp(tyn, "short"))
			{ bt=BS2CC_TYZ_SHORT; }
		else if(!strcmp(tyn, "ushort"))
			{ bt=BS2CC_TYZ_USHORT; }
		else if(!strcmp(tyn, "nlong"))
			{ bt=BS2CC_TYZ_NLONG; }
		else if(!strcmp(tyn, "unlong"))
			{ bt=BS2CC_TYZ_UNLONG; }
		else if(!strcmp(tyn, "var"))
			{ bt=BS2CC_TYZ_VARIANT; }
		else if(!strcmp(tyn, "variant"))
			{ bt=BS2CC_TYZ_VARIANT; }
		else if(!strcmp(tyn, "auto"))
			{ bt=BS2CC_TYZ_AUTOVAR; }
		else if(!strcmp(tyn, "string"))
			{ bt=BS2CC_TYZ_STRING; }
		else if(!strcmp(tyn, "cstring"))
			{ bt=BS2CC_TYZ_CSTRING; }
		else if(!strcmp(tyn, "char"))
			{ bt=BS2CC_TYZ_CHAR; }
		else if(!strcmp(tyn, "char8"))
			{ bt=BS2CC_TYZ_CHAR8; }
		else if(!strcmp(tyn, "bool"))
			{ bt=BS2CC_TYZ_BOOL; }
		else if(!strcmp(tyn, "hfloat"))
			{ bt=BS2CC_TYZ_HFLOAT; }
		else if(!strcmp(tyn, "int128"))
			{ bt=BS2CC_TYZ_INT128; }
		else if(!strcmp(tyn, "uint128"))
			{ bt=BS2CC_TYZ_UINT128; }
		else if(!strcmp(tyn, "float128"))
			{ bt=BS2CC_TYZ_FLOAT128; }
		else if(!strcmp(tyn, "vec2f"))
			{ bt=BS2CC_TYZ_VEC2F; }
		else if(!strcmp(tyn, "vec3f"))
			{ bt=BS2CC_TYZ_VEC3F; }
		else if(!strcmp(tyn, "vec4f"))
			{ bt=BS2CC_TYZ_VEC4F; }
		else if(!strcmp(tyn, "vec2d"))
			{ bt=BS2CC_TYZ_VEC2D; }
		else if(!strcmp(tyn, "quatf"))
			{ bt=BS2CC_TYZ_QUATF; }
		else if(!strcmp(tyn, "fcomplex"))
			{ bt=BS2CC_TYZ_FCPLX; }
		else if(!strcmp(tyn, "dcomplex"))
			{ bt=BS2CC_TYZ_DCPLX; }
		else if(!strcmp(tyn, "vec3d"))
			{ bt=BS2CC_TYZ_VEC3D; }
		else if(!strcmp(tyn, "vec4d"))
			{ bt=BS2CC_TYZ_VEC4D; }
		else if(!strcmp(tyn, "vec3xf"))
			{ bt=BS2CC_TYZ_VEC3XF; }
		else if(!strcmp(tyn, "vec2"))
			{ bt=BS2CC_TYZ_VEC2D; }
		else if(!strcmp(tyn, "vec3"))
			{ bt=BS2CC_TYZ_VEC3XF; }
		else if(!strcmp(tyn, "vec4"))
			{ bt=BS2CC_TYZ_VEC4F; }
		else if(!strcmp(tyn, "quat"))
			{ bt=BS2CC_TYZ_QUATF; }
		else
		{
			i=BS2C_LookupVariGlobal(ctx, vari, tyn);
			if(i>=0)
			{
				vi=ctx->globals[i];
				switch(vi->vitype)
				{
				case BS2CC_VITYPE_GBLVAR:
					if(!(vi->bmfl&BS2CC_TYFL_TYPEDEF))
						BS2C_WarnNotTypedef(ctx, vi->name);
					bt=vi->bty;
					break;
				case BS2CC_VITYPE_GBLFUNC:
					if(!(vi->bmfl&BS2CC_TYFL_TYPEDEF))
						BS2C_WarnNotTypedef(ctx, vi->name);
					bt=i+256;
					break;
				case BS2CC_VITYPE_STRUCT:
				case BS2CC_VITYPE_CLASS:
				case BS2CC_VITYPE_IFACE:
					bt=i+256;
					break;
				
				default:
					if(!(vi->bmfl&BS2CC_TYFL_TYPEDEF))
						BS2C_WarnNotTypedef(ctx, vi->name);
					bt=i+256;
					break;
				}
			}
			else
			{
				bt=BS2CC_TYZ_ADDRESS;
			}
		}
	}else
	{
		bt=BS2CC_TYZ_VARIANT;
	}
	
	memset(&tovf, 0, sizeof(BS2CC_TypeOverflow));
	tovf.base=bt;
	tovf.al=al;
	tovf.pl=pl;
	tovf.rf=rl;
	tovf.an=an;
	tovf.nab=nab;
	for(i=0; i<an; i++)
		tovf.asz[i]=asza[i];

	ty=BS2C_TypeFromTypeOverflow(ctx, &tovf);
	return(ty);
}
