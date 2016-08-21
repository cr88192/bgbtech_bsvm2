// #include <bteifgl.h>


void BSVM2_Op_LDI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=frm->local[op->i0].i; }
void BSVM2_Op_LDL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l=frm->local[op->i0].l; }
void BSVM2_Op_LDF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f=frm->local[op->i0].f; }
void BSVM2_Op_LDD(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d=frm->local[op->i0].d; }
void BSVM2_Op_LDA(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=frm->local[op->i0].a; }

void BSVM2_Op_STI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].i=frm->stack[op->t0].i; }
void BSVM2_Op_STL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].l=frm->stack[op->t0].l; }
void BSVM2_Op_STF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].f=frm->stack[op->t0].f; }
void BSVM2_Op_STD(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].d=frm->stack[op->t0].d; }
void BSVM2_Op_STA(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].a=frm->stack[op->t0].a; }

void BSVM2_Op_LDCI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=op->v.i; }
void BSVM2_Op_LDCL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l=op->v.l; }
void BSVM2_Op_LDCF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f=op->v.f; }
void BSVM2_Op_LDCD(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d=op->v.d; }
void BSVM2_Op_LDCA(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=op->v.a; }

void BSVM2_Op_MVI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].i=frm->local[op->i1].i; }
void BSVM2_Op_MVL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].l=frm->local[op->i1].l; }
void BSVM2_Op_MVF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].f=frm->local[op->i1].f; }
void BSVM2_Op_MVD(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].d=frm->local[op->i1].d; }
void BSVM2_Op_MVA(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].a=frm->local[op->i1].a; }

void BSVM2_Op_MVIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].i=op->v.i; }
void BSVM2_Op_MVLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].l=op->v.l; }
void BSVM2_Op_MVFC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].f=op->v.f; }
void BSVM2_Op_MVDC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].d=op->v.d; }
void BSVM2_Op_MVAC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].a=op->v.a; }


void BSVM2_Op_LDGSI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	frm->stack[op->t0].i=vi->gvalue->i;		}
void BSVM2_Op_LDGSL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	frm->stack[op->t0].l=vi->gvalue->l;		}
void BSVM2_Op_LDGSF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	frm->stack[op->t0].f=vi->gvalue->f;		}
void BSVM2_Op_LDGSD(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	frm->stack[op->t0].d=vi->gvalue->d;		}
void BSVM2_Op_LDGSA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	frm->stack[op->t0].a=vi->gvalue->a;		}

void BSVM2_Op_STGSI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	vi->gvalue->i=frm->stack[op->t0].i;		}
void BSVM2_Op_STGSL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	vi->gvalue->l=frm->stack[op->t0].l;		}
void BSVM2_Op_STGSF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	vi->gvalue->f=frm->stack[op->t0].f;		}
void BSVM2_Op_STGSD(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	vi->gvalue->d=frm->stack[op->t0].d;		}
void BSVM2_Op_STGSA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	vi->gvalue->a=frm->stack[op->t0].a;		}

#if 1
void BSVM2_Op_LDGS_DY(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ImageGlobal *vi;
	
	if(!frm->ctx->dynenv)
	{
		frm->stack[op->t0].l=0;
		return;
	}
	
	vi=op->v.p;
	if((vi->nargs<=0) || (vi->nargs>=frm->ctx->szdynenv))
	{
		frm->stack[op->t0].l=0;
		return;
	}
	
	switch(op->i1)
	{
	case BSVM2_OPZ_INT:		case BSVM2_OPZ_UINT:
	case BSVM2_OPZ_SBYTE:	case BSVM2_OPZ_UBYTE:
	case BSVM2_OPZ_SHORT:	case BSVM2_OPZ_USHORT:
		frm->stack[op->t0].i=frm->ctx->dynenv[vi->nargs].i;
		break;
	case BSVM2_OPZ_LONG:	case BSVM2_OPZ_ULONG:
		frm->stack[op->t0].l=frm->ctx->dynenv[vi->nargs].l;
		break;
	case BSVM2_OPZ_FLOAT:
		frm->stack[op->t0].f=frm->ctx->dynenv[vi->nargs].f;
		break;
	case BSVM2_OPZ_DOUBLE:
		frm->stack[op->t0].d=frm->ctx->dynenv[vi->nargs].d;
		break;
	case BSVM2_OPZ_ADDRESS:
		frm->stack[op->t0].a=frm->ctx->dynenv[vi->nargs].a;
		break;
	}
}

void BSVM2_Op_STGS_DY(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ImageGlobal *vi;
	
	if(!frm->ctx->dynenv)
	{
		frm->ctx->dynenv=dtmAlloc("bs2i_dynenv_t",
			bs2i_img_dynvlim*sizeof(BSVM2_Value));
		frm->ctx->szdynenv=bs2i_img_dynvlim;
	}

	vi=op->v.p;
	if((vi->nargs<=0) || (vi->nargs>=bs2i_img_dynvlim))
		return;
	if((vi->nargs>=frm->ctx->szdynenv))
	{
		frm->ctx->dynenv=dtmRealloc(frm->ctx->dynenv,
			bs2i_img_dynvlim*sizeof(BSVM2_Value));
		frm->ctx->szdynenv=bs2i_img_dynvlim;
	}
	
	switch(op->i1)
	{
	case BSVM2_OPZ_INT:		case BSVM2_OPZ_UINT:
	case BSVM2_OPZ_SBYTE:	case BSVM2_OPZ_UBYTE:
	case BSVM2_OPZ_SHORT:	case BSVM2_OPZ_USHORT:
		frm->ctx->dynenv[vi->nargs].i=frm->stack[op->t0].i;
		break;
	case BSVM2_OPZ_LONG:	case BSVM2_OPZ_ULONG:
		frm->ctx->dynenv[vi->nargs].l=frm->stack[op->t0].l;
		break;
	case BSVM2_OPZ_FLOAT:
		frm->ctx->dynenv[vi->nargs].f=frm->stack[op->t0].f;
		break;
	case BSVM2_OPZ_DOUBLE:
		frm->ctx->dynenv[vi->nargs].d=frm->stack[op->t0].d;
		break;
	case BSVM2_OPZ_ADDRESS:
		frm->ctx->dynenv[vi->nargs].a=frm->stack[op->t0].a;
		break;
	}
}
#endif

void BSVM2_Op_LDCTH(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=frm->self; }
