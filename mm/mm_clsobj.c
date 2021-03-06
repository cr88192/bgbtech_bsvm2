/*
 * Stuff for Class/Instance object system.
 */

// #include <bteifgl.h>

BGBDTC_ClassInfo *bgbdtc_clsobj_root;

BGBDTC_ClassInfo *BGBDTC_AllocClass()
{
	BGBDTC_ClassInfo *tmp;
	
	tmp=dtmAlloc("bgbdtc_classinfo_t", sizeof(BGBDTC_ClassInfo));
	return(tmp);
}

BS2VM_API BGBDTC_ClassInfo *BGBDTC_LookupClassQName(char *qname)
{
	BGBDTC_ClassInfo *cur;
	
	cur=bgbdtc_clsobj_root;
	while(cur)
	{
		if(cur->qname && !strcmp(cur->qname, qname))
			return(cur);
		cur=cur->next;
	}
	return(NULL);
}

BS2VM_API BGBDTC_ClassInfo *BGBDTC_GetClassQName(char *qname)
{
	BGBDTC_ClassInfo *cur;

	cur=BGBDTC_LookupClassQName(qname);
	if(cur)return(cur);
	
	cur=BGBDTC_AllocClass();
	cur->qname=BGBDT_TagStr_StrSymbol(qname);
	
	cur->next=bgbdtc_clsobj_root;
	bgbdtc_clsobj_root=cur;
	
	return(cur);
}


BGBDTC_ClassInfo *BGBDTC_LookupClassSqGix(int sqid, int gix)
{
	BGBDTC_ClassInfo *cur;
	
	cur=bgbdtc_clsobj_root;
	while(cur)
	{
		if((cur->sqid==sqid) && (cur->gix==gix))
			return(cur);
		cur=cur->next;
	}
	return(NULL);
}

BGBDTC_ClassInfo *BGBDTC_GetClassSqGix(int sqid, int gix)
{
	BGBDTC_ClassInfo *cur;

	cur=BGBDTC_LookupClassSqGix(sqid, gix);
	if(cur)return(cur);
	
	cur=BGBDTC_AllocClass();
	cur->sqid=sqid;
	cur->gix=gix;
//	cur->qname=BGBDT_TagStr_StrSymbol(qname);
	
	cur->next=bgbdtc_clsobj_root;
	bgbdtc_clsobj_root=cur;
	
	return(cur);
}

void *BGBDTC_SlotI_GetPtr_Dfl(dtcObject obj, dtcField fi)
{
	return(((byte *)obj)+fi->offs);
}


s32 BGBDTC_SlotI_GetI_DflSB(dtcObject obj, dtcField fi)
	{ return(*(sbyte *)(((byte *)obj)+fi->offs)); }
s32 BGBDTC_SlotI_GetI_DflUB(dtcObject obj, dtcField fi)
	{ return(*(byte *)(((byte *)obj)+fi->offs)); }
s32 BGBDTC_SlotI_GetI_DflSS(dtcObject obj, dtcField fi)
	{ return(*(s16 *)(((byte *)obj)+fi->offs)); }
s32 BGBDTC_SlotI_GetI_DflUS(dtcObject obj, dtcField fi)
	{ return(*(u16 *)(((byte *)obj)+fi->offs)); }
s32 BGBDTC_SlotI_GetI_DflI(dtcObject obj, dtcField fi)
	{ return(*(s32 *)(((byte *)obj)+fi->offs)); }
s32 BGBDTC_SlotI_GetI_DeL(dtcObject obj, dtcField fi)
	{ return(fi->GetL(obj, fi)); }

s64 BGBDTC_SlotI_GetL_DflSI(dtcObject obj, dtcField fi)
	{ return(*(s32 *)(((byte *)obj)+fi->offs)); }
s64 BGBDTC_SlotI_GetL_DflUI(dtcObject obj, dtcField fi)
	{ return(*(u32 *)(((byte *)obj)+fi->offs)); }

s64 BGBDTC_SlotI_GetL_DflL(dtcObject obj, dtcField fi)
	{ return(*(s64 *)(((byte *)obj)+fi->offs)); }
s64 BGBDTC_SlotI_GetL_DeI(dtcObject obj, dtcField fi)
	{ return(fi->GetI(obj, fi)); }
s64 BGBDTC_SlotI_GetL_DeF(dtcObject obj, dtcField fi)
	{ return(fi->GetF(obj, fi)); }
s64 BGBDTC_SlotI_GetL_DeD(dtcObject obj, dtcField fi)
	{ return(fi->GetD(obj, fi)); }

f32 BGBDTC_SlotI_GetF_DflF(dtcObject obj, dtcField fi)
	{ return(*(f32 *)(((byte *)obj)+fi->offs)); }
f32 BGBDTC_SlotI_GetF_DflD(dtcObject obj, dtcField fi)
	{ return(*(f64 *)(((byte *)obj)+fi->offs)); }
f32 BGBDTC_SlotI_GetF_DeL(dtcObject obj, dtcField fi)
	{ return(fi->GetL(obj, fi)); }

f64 BGBDTC_SlotI_GetD_DflF(dtcObject obj, dtcField fi)
	{ return(*(f32 *)(((byte *)obj)+fi->offs)); }
f64 BGBDTC_SlotI_GetD_DflD(dtcObject obj, dtcField fi)
	{ return(*(f64 *)(((byte *)obj)+fi->offs)); }
f64 BGBDTC_SlotI_GetD_DeL(dtcObject obj, dtcField fi)
	{ return(fi->GetL(obj, fi)); }

dtVal BGBDTC_SlotI_GetA_DflA(dtcObject obj, dtcField fi)
	{ return(*(dtVal *)(((byte *)obj)+fi->offs)); }
dtVal BGBDTC_SlotI_GetA_DflP(dtcObject obj, dtcField fi)
	{ return(dtvWrapPtr(*(void **)(((byte *)obj)+fi->offs))); }

dtVal BGBDTC_SlotI_GetA_DflPF(dtcObject obj, dtcField fi)
	{ return(dtvWrapTyTagPtrF(*(void **)(((byte *)obj)+fi->offs), 0)); }

dtVal BGBDTC_SlotI_GetA_DflArr(dtcObject obj, dtcField fi)
	{ return(dtvWrapTyTagPtrF((((byte *)obj)+fi->offs), 0)); }

u16 bgbdtc_bswap_us(u16 v)
{
	return(((v>>8)&0x00FF)|((v<<8)&0xFF00));
}

u32 bgbdtc_bswap_ui(u32 v)
{
	return(((v>>24)&0x000000FF)|
			((v>> 8)&0x0000FF00)|
			((v<< 8)&0x00FF0000)|
			((v<<24)&0xFF000000));
}

u64 bgbdtc_bswap_ul(u64 v)
{
	return( ((u64)bgbdtc_bswap_ui(v>>32))|
			(((u64)bgbdtc_bswap_ui(v))<<32));
}

s16 bgbdtc_bswap_ss(s16 v)
	{ return((s16)(bgbdtc_bswap_us((u16)v))); }
s32 bgbdtc_bswap_si(s32 v)
	{ return((s32)(bgbdtc_bswap_ui((u32)v))); }
s64 bgbdtc_bswap_sl(s64 v)
	{ return((s64)(bgbdtc_bswap_ul((u64)v))); }

s32 BGBDTC_SlotI_GetI_DflSwSS(dtcObject obj, dtcField fi)
	{ return(bgbdtc_bswap_ss(*(s16 *)(((byte *)obj)+fi->offs))); }
s32 BGBDTC_SlotI_GetI_DflSwUS(dtcObject obj, dtcField fi)
	{ return(bgbdtc_bswap_us(*(u16 *)(((byte *)obj)+fi->offs))); }
s32 BGBDTC_SlotI_GetI_DflSwI(dtcObject obj, dtcField fi)
	{ return(bgbdtc_bswap_si(*(s32 *)(((byte *)obj)+fi->offs))); }

s64 BGBDTC_SlotI_GetL_DflSwSI(dtcObject obj, dtcField fi)
	{ return(bgbdtc_bswap_si(*(s32 *)(((byte *)obj)+fi->offs))); }
s64 BGBDTC_SlotI_GetL_DflSwUI(dtcObject obj, dtcField fi)
	{ return(bgbdtc_bswap_ui(*(u32 *)(((byte *)obj)+fi->offs))); }
s64 BGBDTC_SlotI_GetL_DflSwL(dtcObject obj, dtcField fi)
	{ return(bgbdtc_bswap_sl(*(s64 *)(((byte *)obj)+fi->offs))); }


void BGBDTC_SlotI_SetI_DflSwS(dtcObject obj, dtcField fi, s32 v)
	{ *(s16 *)(((byte *)obj)+fi->offs)=bgbdtc_bswap_ss(v); }
void BGBDTC_SlotI_SetI_DflSwI(dtcObject obj, dtcField fi, s32 v)
	{ *(s32 *)(((byte *)obj)+fi->offs)=bgbdtc_bswap_si(v); }
void BGBDTC_SlotI_SetL_DflSwL(dtcObject obj, dtcField fi, s64 v)
	{ *(s64 *)(((byte *)obj)+fi->offs)=bgbdtc_bswap_sl(v); }


dtVal BGBDTC_SlotI_GetA_DeI(dtcObject obj, dtcField fi)
	{ return(dtvWrapInt(fi->GetI(obj, fi))); }
dtVal BGBDTC_SlotI_GetA_DeUI(dtcObject obj, dtcField fi)
	{ return(dtvWrapUInt(fi->GetI(obj, fi))); }
dtVal BGBDTC_SlotI_GetA_DeL(dtcObject obj, dtcField fi)
	{ return(dtvWrapLong(fi->GetL(obj, fi))); }
dtVal BGBDTC_SlotI_GetA_DeF(dtcObject obj, dtcField fi)
	{ return(dtvWrapFloat(fi->GetF(obj, fi))); }
dtVal BGBDTC_SlotI_GetA_DeD(dtcObject obj, dtcField fi)
	{ return(dtvWrapDouble(fi->GetD(obj, fi))); }

s32 BGBDTC_SlotI_GetI_DeA(dtcObject obj, dtcField fi)
	{ return(dtvUnwrapInt(fi->GetA(obj, fi))); }
s64 BGBDTC_SlotI_GetL_DeA(dtcObject obj, dtcField fi)
	{ return(dtvUnwrapLong(fi->GetA(obj, fi))); }
f32 BGBDTC_SlotI_GetF_DeA(dtcObject obj, dtcField fi)
	{ return(dtvUnwrapFloat(fi->GetA(obj, fi))); }
f64 BGBDTC_SlotI_GetD_DeA(dtcObject obj, dtcField fi)
	{ return(dtvUnwrapDouble(fi->GetA(obj, fi))); }

void BGBDTC_SlotI_SetI_DflB(dtcObject obj, dtcField fi, s32 v)
	{ *(byte *)(((byte *)obj)+fi->offs)=v; }
void BGBDTC_SlotI_SetI_DflS(dtcObject obj, dtcField fi, s32 v)
	{ *(s16 *)(((byte *)obj)+fi->offs)=v; }

void BGBDTC_SlotI_SetI_DflI(dtcObject obj, dtcField fi, s32 v)
	{ *(s32 *)(((byte *)obj)+fi->offs)=v; }
void BGBDTC_SlotI_SetL_DflL(dtcObject obj, dtcField fi, s64 v)
	{ *(s64 *)(((byte *)obj)+fi->offs)=v; }
void BGBDTC_SlotI_SetF_DflF(dtcObject obj, dtcField fi, f32 v)
	{ *(f32 *)(((byte *)obj)+fi->offs)=v; }
void BGBDTC_SlotI_SetD_DflD(dtcObject obj, dtcField fi, f64 v)
	{ *(f64 *)(((byte *)obj)+fi->offs)=v; }
void BGBDTC_SlotI_SetA_DflA(dtcObject obj, dtcField fi, dtVal v)
	{ *(dtVal *)(((byte *)obj)+fi->offs)=v; }
void BGBDTC_SlotI_SetA_DflP(dtcObject obj, dtcField fi, dtVal v)
	{ *(void **)(((byte *)obj)+fi->offs)=dtvUnwrapPtr(v); }

void BGBDTC_SlotI_SetA_DflPF(dtcObject obj, dtcField fi, dtVal v)
	{ *(void **)(((byte *)obj)+fi->offs)=dtvUnwrapPtrF(v); }

void BGBDTC_SlotI_SetI_DeL(dtcObject obj, dtcField fi, s32 v)
	{ fi->SetL(obj, fi, v); }
void BGBDTC_SlotI_SetI_DeF(dtcObject obj, dtcField fi, s32 v)
	{ fi->SetF(obj, fi, v); }
void BGBDTC_SlotI_SetI_DeD(dtcObject obj, dtcField fi, s32 v)
	{ fi->SetD(obj, fi, v); }

void BGBDTC_SlotI_SetL_DeI(dtcObject obj, dtcField fi, s64 v)
	{ fi->SetI(obj, fi, v); }
void BGBDTC_SlotI_SetL_DeF(dtcObject obj, dtcField fi, s64 v)
	{ fi->SetF(obj, fi, v); }
void BGBDTC_SlotI_SetL_DeD(dtcObject obj, dtcField fi, s64 v)
	{ fi->SetD(obj, fi, v); }

void BGBDTC_SlotI_SetF_DeI(dtcObject obj, dtcField fi, f32 v)
	{ fi->SetI(obj, fi, v); }
void BGBDTC_SlotI_SetF_DeL(dtcObject obj, dtcField fi, f32 v)
	{ fi->SetL(obj, fi, v); }
void BGBDTC_SlotI_SetF_DeD(dtcObject obj, dtcField fi, f32 v)
	{ fi->SetD(obj, fi, v); }

void BGBDTC_SlotI_SetD_DeI(dtcObject obj, dtcField fi, f64 v)
	{ fi->SetI(obj, fi, v); }
void BGBDTC_SlotI_SetD_DeL(dtcObject obj, dtcField fi, f64 v)
	{ fi->SetL(obj, fi, v); }
void BGBDTC_SlotI_SetD_DeF(dtcObject obj, dtcField fi, f64 v)
	{ fi->SetF(obj, fi, v); }


void BGBDTC_SlotI_SetA_DeI(dtcObject obj, dtcField fi, dtVal v)
	{ fi->SetI(obj, fi, dtvUnwrapInt(v)); }
void BGBDTC_SlotI_SetA_DeL(dtcObject obj, dtcField fi, dtVal v)
	{ fi->SetL(obj, fi, dtvUnwrapLong(v)); }
void BGBDTC_SlotI_SetA_DeF(dtcObject obj, dtcField fi, dtVal v)
	{ fi->SetF(obj, fi, dtvUnwrapFloat(v)); }
void BGBDTC_SlotI_SetA_DeD(dtcObject obj, dtcField fi, dtVal v)
	{ fi->SetD(obj, fi, dtvUnwrapDouble(v)); }

void BGBDTC_SlotI_SetI_DeA(dtcObject obj, dtcField fi, s32 v)
	{ fi->SetA(obj, fi, dtvWrapInt(v)); }
void BGBDTC_SlotI_SetL_DeA(dtcObject obj, dtcField fi, s64 v)
	{ fi->SetA(obj, fi, dtvWrapLong(v)); }
void BGBDTC_SlotI_SetF_DeA(dtcObject obj, dtcField fi, f32 v)
	{ fi->SetA(obj, fi, dtvWrapFloat(v)); }
void BGBDTC_SlotI_SetD_DeA(dtcObject obj, dtcField fi, f64 v)
	{ fi->SetA(obj, fi, dtvWrapDouble(v)); }


dtVal BGBDTC_SlotI_GetA_ClsA(dtcObject obj, dtcField fi)
{
	BGBDTC_ClassInfo *ci;
	ci=*(BGBDTC_ClassInfo **)obj;
	return(*(dtVal *)(ci->cldata+fi->offs));
}

void BGBDTC_SlotI_SetA_ClsA(dtcObject obj, dtcField fi, dtVal v)
{
	BGBDTC_ClassInfo *ci;
	ci=*(BGBDTC_ClassInfo **)obj;
	*(dtVal *)(ci->cldata+fi->offs)=v;
}

dtVal BGBDTC_SlotI_GetA_StrA(dtcObject obj, dtcField fi)
	{ return(fi->init); }
void BGBDTC_SlotI_SetA_StrA(dtcObject obj, dtcField fi, dtVal v)
	{ fi->init=v; }

dtVal BGBDTC_SlotI_GetA_IfaceA(dtcObject obj, dtcField fi)
{
	BGBDTC_ClassInfo *ci;
	BGBDTC_SlotInfo *vi, *vi2;

	ci=*(BGBDTC_ClassInfo **)obj;
	vi=fi;
	vi2=BGBDTC_LookupClassMatchSlot(ci, vi);
	if(!vi2)
		return(DTV_UNDEFINED);
	return(*(dtVal *)(ci->cldata+vi2->offs));
//	return(fi->init);
}

void BGBDTC_SlotI_SetA_IfaceA(dtcObject obj, dtcField fi, dtVal v)
{
//	fi->init=v;
}


int BGBDTC_SlotSetupVtGetSet(
	BGBDTC_ClassInfo *cls,
	BGBDTC_SlotInfo *vi)
{
	vi->GetPtr=BGBDTC_SlotI_GetPtr_Dfl;
	
	switch(*vi->sig)
	{
	case 'a':	case 'b':	case 'c':
		vi->GetI=BGBDTC_SlotI_GetI_DflSB;
		vi->GetL=BGBDTC_SlotI_GetL_DeI;
		vi->GetF=BGBDTC_SlotI_GetF_DeL;
		vi->GetD=BGBDTC_SlotI_GetD_DeL;
		vi->GetA=BGBDTC_SlotI_GetA_DeI;
		vi->SetI=BGBDTC_SlotI_SetI_DflB;
		vi->SetL=BGBDTC_SlotI_SetL_DeI;
		vi->SetF=BGBDTC_SlotI_SetF_DeI;
		vi->SetD=BGBDTC_SlotI_SetD_DeI;
		vi->SetA=BGBDTC_SlotI_SetA_DeI;
		break;
	
	case 'h':
		vi->GetI=BGBDTC_SlotI_GetI_DflUB;
		vi->GetL=BGBDTC_SlotI_GetL_DeI;
		vi->GetF=BGBDTC_SlotI_GetF_DeL;
		vi->GetD=BGBDTC_SlotI_GetD_DeL;
		vi->GetA=BGBDTC_SlotI_GetA_DeI;
		vi->SetI=BGBDTC_SlotI_SetI_DflB;
		vi->SetL=BGBDTC_SlotI_SetL_DeI;
		vi->SetF=BGBDTC_SlotI_SetF_DeI;
		vi->SetD=BGBDTC_SlotI_SetD_DeI;
		vi->SetA=BGBDTC_SlotI_SetA_DeI;
		break;

	case 's':
		vi->GetI=BGBDTC_SlotI_GetI_DflSS;
		vi->GetL=BGBDTC_SlotI_GetL_DeI;
		vi->GetF=BGBDTC_SlotI_GetF_DeL;
		vi->GetD=BGBDTC_SlotI_GetD_DeL;
		vi->GetA=BGBDTC_SlotI_GetA_DeI;
		vi->SetI=BGBDTC_SlotI_SetI_DflS;
		vi->SetL=BGBDTC_SlotI_SetL_DeI;
		vi->SetF=BGBDTC_SlotI_SetF_DeI;
		vi->SetD=BGBDTC_SlotI_SetD_DeI;
		vi->SetA=BGBDTC_SlotI_SetA_DeI;

		if(vi->slotfl&BGBDTC_SFL_ENBSW)
		{
			vi->GetI=BGBDTC_SlotI_GetI_DflSwSS;
			vi->SetI=BGBDTC_SlotI_SetI_DflSwS;
		}
		break;
	case 't':	case 'w':
		vi->GetI=BGBDTC_SlotI_GetI_DflUS;
		vi->GetL=BGBDTC_SlotI_GetL_DeI;
		vi->GetF=BGBDTC_SlotI_GetF_DeL;
		vi->GetD=BGBDTC_SlotI_GetD_DeL;
		vi->GetA=BGBDTC_SlotI_GetA_DeI;
		vi->SetI=BGBDTC_SlotI_SetI_DflS;
		vi->SetL=BGBDTC_SlotI_SetL_DeI;
		vi->SetF=BGBDTC_SlotI_SetF_DeI;
		vi->SetD=BGBDTC_SlotI_SetD_DeI;
		vi->SetA=BGBDTC_SlotI_SetA_DeI;

		if(vi->slotfl&BGBDTC_SFL_ENBSW)
		{
			vi->GetI=BGBDTC_SlotI_GetI_DflSwUS;
			vi->SetI=BGBDTC_SlotI_SetI_DflSwS;
		}
		break;
	case 'i':
		vi->GetI=BGBDTC_SlotI_GetI_DflI;
		vi->GetL=BGBDTC_SlotI_GetL_DflSI;
		vi->GetF=BGBDTC_SlotI_GetF_DeL;
		vi->GetD=BGBDTC_SlotI_GetD_DeL;
		vi->GetA=BGBDTC_SlotI_GetA_DeI;
		vi->SetI=BGBDTC_SlotI_SetI_DflI;
		vi->SetL=BGBDTC_SlotI_SetL_DeI;
		vi->SetF=BGBDTC_SlotI_SetF_DeI;
		vi->SetD=BGBDTC_SlotI_SetD_DeI;
		vi->SetA=BGBDTC_SlotI_SetA_DeI;

		if(vi->slotfl&BGBDTC_SFL_ENBSW)
		{
			vi->GetI=BGBDTC_SlotI_GetI_DflSwI;
			vi->GetL=BGBDTC_SlotI_GetL_DflSwSI;
			vi->SetI=BGBDTC_SlotI_SetI_DflSwI;
		}
		break;
	case 'j':
		vi->GetI=BGBDTC_SlotI_GetI_DflI;
		vi->GetL=BGBDTC_SlotI_GetL_DflUI;
		vi->GetF=BGBDTC_SlotI_GetF_DeL;
		vi->GetD=BGBDTC_SlotI_GetD_DeL;
		vi->GetA=BGBDTC_SlotI_GetA_DeUI;
		vi->SetI=BGBDTC_SlotI_SetI_DflI;
		vi->SetL=BGBDTC_SlotI_SetL_DeI;
		vi->SetF=BGBDTC_SlotI_SetF_DeI;
		vi->SetD=BGBDTC_SlotI_SetD_DeI;
		vi->SetA=BGBDTC_SlotI_SetA_DeI;

		if(vi->slotfl&BGBDTC_SFL_ENBSW)
		{
			vi->GetI=BGBDTC_SlotI_GetI_DflSwI;
			vi->GetL=BGBDTC_SlotI_GetL_DflSwUI;
			vi->SetI=BGBDTC_SlotI_SetI_DflSwI;
		}
		break;

	case 'l':
#if defined(X86) || defined(ARM)
		vi->GetI=BGBDTC_SlotI_GetI_DflI;
		vi->GetL=BGBDTC_SlotI_GetL_DflSI;
		vi->SetI=BGBDTC_SlotI_SetI_DflI;
		vi->SetL=BGBDTC_SlotI_SetL_DeI;
#else
		vi->GetI=BGBDTC_SlotI_GetI_DeL;
		vi->GetL=BGBDTC_SlotI_GetL_DflL;
		vi->SetI=BGBDTC_SlotI_SetI_DeL;
		vi->SetL=BGBDTC_SlotI_SetL_DflL;
#endif
		vi->GetF=BGBDTC_SlotI_GetF_DeL;
		vi->GetD=BGBDTC_SlotI_GetD_DeL;
		vi->GetA=BGBDTC_SlotI_GetA_DeL;
		vi->SetF=BGBDTC_SlotI_SetF_DeL;
		vi->SetD=BGBDTC_SlotI_SetD_DeL;
		vi->SetA=BGBDTC_SlotI_SetA_DeL;
		break;

	case 'm':
#if defined(X86) || defined(ARM)
		vi->GetI=BGBDTC_SlotI_GetI_DflI;
		vi->GetL=BGBDTC_SlotI_GetL_DflUI;
		vi->SetI=BGBDTC_SlotI_SetI_DflI;
		vi->SetL=BGBDTC_SlotI_SetL_DeI;
#else
		vi->GetI=BGBDTC_SlotI_GetI_DeL;
		vi->GetL=BGBDTC_SlotI_GetL_DflL;
		vi->SetI=BGBDTC_SlotI_SetI_DeL;
		vi->SetL=BGBDTC_SlotI_SetL_DflL;
#endif
		vi->GetF=BGBDTC_SlotI_GetF_DeL;
		vi->GetD=BGBDTC_SlotI_GetD_DeL;
		vi->GetA=BGBDTC_SlotI_GetA_DeL;
		vi->SetF=BGBDTC_SlotI_SetF_DeL;
		vi->SetD=BGBDTC_SlotI_SetD_DeL;
		vi->SetA=BGBDTC_SlotI_SetA_DeL;
		break;

	case 'x':	case 'y':
		vi->GetI=BGBDTC_SlotI_GetI_DeL;
		vi->GetL=BGBDTC_SlotI_GetL_DflL;
		vi->GetF=BGBDTC_SlotI_GetF_DeL;
		vi->GetD=BGBDTC_SlotI_GetD_DeL;
		vi->GetA=BGBDTC_SlotI_GetA_DeL;

		vi->SetI=BGBDTC_SlotI_SetI_DeL;
		vi->SetL=BGBDTC_SlotI_SetL_DflL;
		vi->SetF=BGBDTC_SlotI_SetF_DeL;
		vi->SetD=BGBDTC_SlotI_SetD_DeL;
		vi->SetA=BGBDTC_SlotI_SetA_DeL;

		if(vi->slotfl&BGBDTC_SFL_ENBSW)
		{
			vi->GetL=BGBDTC_SlotI_GetL_DflSwL;
			vi->SetL=BGBDTC_SlotI_SetL_DflSwL;
		}
		break;

	case 'f':
		vi->GetI=BGBDTC_SlotI_GetI_DeL;
		vi->GetL=BGBDTC_SlotI_GetL_DeF;
		vi->GetF=BGBDTC_SlotI_GetF_DflF;
		vi->GetD=BGBDTC_SlotI_GetD_DflF;
		vi->GetA=BGBDTC_SlotI_GetA_DeF;
		vi->SetI=BGBDTC_SlotI_SetI_DeF;
		vi->SetL=BGBDTC_SlotI_SetL_DeF;
		vi->SetF=BGBDTC_SlotI_SetF_DflF;
		vi->SetD=BGBDTC_SlotI_SetD_DeF;
		vi->SetA=BGBDTC_SlotI_SetA_DeF;
		break;
	case 'd':
	case 'e':
		vi->GetI=BGBDTC_SlotI_GetI_DeL;
		vi->GetL=BGBDTC_SlotI_GetL_DeD;
		vi->GetF=BGBDTC_SlotI_GetF_DflD;
		vi->GetD=BGBDTC_SlotI_GetD_DflD;
		vi->GetA=BGBDTC_SlotI_GetA_DeD;
		vi->SetI=BGBDTC_SlotI_SetI_DeD;
		vi->SetL=BGBDTC_SlotI_SetL_DeD;
		vi->SetF=BGBDTC_SlotI_SetF_DeD;
		vi->SetD=BGBDTC_SlotI_SetD_DflD;
		vi->SetA=BGBDTC_SlotI_SetA_DeD;
		break;

	case 'p':	case 'q':	case 'r':
	case 'Q':	case 'L':
	case 'C':
	case 'X':
//	case 'A':
		vi->GetI=BGBDTC_SlotI_GetI_DeA;
		vi->GetL=BGBDTC_SlotI_GetL_DeA;
		vi->GetF=BGBDTC_SlotI_GetF_DeA;
		vi->GetD=BGBDTC_SlotI_GetD_DeA;
		vi->GetA=BGBDTC_SlotI_GetA_DflA;

		vi->SetI=BGBDTC_SlotI_SetI_DeA;
		vi->SetL=BGBDTC_SlotI_SetL_DeA;
		vi->SetF=BGBDTC_SlotI_SetF_DeA;
		vi->SetD=BGBDTC_SlotI_SetD_DeA;
		vi->SetA=BGBDTC_SlotI_SetA_DflA;
		break;

	case 'A':
//		vi->GetI=BGBDTC_SlotI_GetI_DeA;
//		vi->GetL=BGBDTC_SlotI_GetL_DeA;
//		vi->GetF=BGBDTC_SlotI_GetF_DeA;
//		vi->GetD=BGBDTC_SlotI_GetD_DeA;
		vi->GetA=BGBDTC_SlotI_GetA_DflArr;

//		vi->SetI=BGBDTC_SlotI_SetI_DeA;
//		vi->SetL=BGBDTC_SlotI_SetL_DeA;
//		vi->SetF=BGBDTC_SlotI_SetF_DeA;
//		vi->SetD=BGBDTC_SlotI_SetD_DeA;
//		vi->SetA=BGBDTC_SlotI_SetA_DflA;
		break;

	case 'P':
		vi->GetI=BGBDTC_SlotI_GetI_DeA;
		vi->GetL=BGBDTC_SlotI_GetL_DeA;
		vi->GetF=BGBDTC_SlotI_GetF_DeA;
		vi->GetD=BGBDTC_SlotI_GetD_DeA;
		vi->GetA=BGBDTC_SlotI_GetA_DflP;

		vi->SetI=BGBDTC_SlotI_SetI_DeA;
		vi->SetL=BGBDTC_SlotI_SetL_DeA;
		vi->SetF=BGBDTC_SlotI_SetF_DeA;
		vi->SetD=BGBDTC_SlotI_SetD_DeA;
		vi->SetA=BGBDTC_SlotI_SetA_DflP;
		
		switch(vi->sig[1])
		{
		case 'a':	case 'b':	case 'c':	case 'd':
		case 'e':	case 'f':	case 'g':	case 'h':
		case 'i':	case 'j':	case 'k':	case 'l':
		case 'm':	case 'n':	case 'o':
		case 's':	case 't':
		case 'w':	case 'x':	case 'y':
			vi->GetA=BGBDTC_SlotI_GetA_DflPF;
			vi->SetA=BGBDTC_SlotI_SetA_DflPF;
			break;
		}
		break;
	default:
		BSVM2_DBGTRAP
		break;
	}

	return(0);
}

BS2VM_API int BGBDTC_FinishLayoutClass(
	BGBDTC_ClassInfo *cls)
{
	BGBDTC_SlotInfo *vi, *vi2;
	int osz, oal, sz, al;
	int clsz;
	int i, j, k;

	if(!cls)
		return(0);
	if(cls->clean)
		return(0);
	cls->clean=2;
	
	if(cls->super)
	{
		BGBDTC_FinishLayoutClass(cls->super);
		osz=cls->super->szdata;
		oal=cls->super->aldata;
		clsz=cls->super->szcldata;
	}else
	{
		if(cls->clsty==BGBDTC_CTY_STRUCT)
		{
			osz=0; oal=1;
			clsz=0;
		}else
		{
			osz=sizeof(void *); oal=osz;
			clsz=0;
		}
	}
	
	for(i=0; i<cls->nslots; i++)
	{
		vi=cls->slots[i];
		
		if(vi->slotty==BGBDTC_STY_FIELD)
		{
			BGBDTC_GetSigSizeAlign(vi->sig, &sz, &al);
			if(sz<0)continue;

			osz=(osz+(al-1))&(~(al-1));
			vi->offs=osz;
		
			osz=osz+sz;
			if(al>oal)oal=al;
		
			BGBDTC_SlotSetupVtGetSet(cls, vi);
		}

		if(vi->slotty==BGBDTC_STY_METHOD)
		{
			vi2=BGBDTC_LookupClassMatchSlot(cls->super, vi);

			if(vi2)
			{
				vi->offs=vi2->offs;
			}else
			{
				sz=8;	al=8;
				clsz=(clsz+(al-1))&(~(al-1));
				vi->offs=clsz;
				clsz=clsz+sz;
			}
			
			if(cls->clsty==BGBDTC_CTY_STRUCT)
			{
				vi->GetA=BGBDTC_SlotI_GetA_StrA;
				vi->SetA=BGBDTC_SlotI_SetA_StrA;
			}else if(cls->clsty==BGBDTC_CTY_CLASS)
			{
				vi->GetA=BGBDTC_SlotI_GetA_ClsA;
				vi->SetA=BGBDTC_SlotI_SetA_ClsA;
			}else if(cls->clsty==BGBDTC_CTY_IFACE)
			{
				vi->GetA=BGBDTC_SlotI_GetA_IfaceA;
				vi->SetA=BGBDTC_SlotI_SetA_IfaceA;
			}else
			{
				BSVM2_DBGTRAP
			}
		}
	}

	osz=(osz+(oal-1))&(~(oal-1));
	cls->szdata=osz;
	cls->aldata=oal;
	cls->clean=1;
	cls->szcldata=clsz;
	
	if(!cls->cldata)
	{
		cls->cldata=dtmAlloc("bgbdtc_classdata_t", cls->szcldata);
		if(cls->super && cls->super->cldata)
		{
			memcpy(cls->cldata, cls->super->cldata,
				cls->super->szcldata);
		}

		for(i=0; i<cls->nslots; i++)
		{
			vi=cls->slots[i];
			if(vi->slotty==BGBDTC_STY_METHOD)
				{ *(dtVal *)(cls->cldata+vi->offs)=vi->init; }
		}
	}
	
	return(1);
}

BS2VM_API int BGBDTC_BeginLayoutClass(
	BGBDTC_ClassInfo *cls)
{
	cls->clean=0;
	return(0);
}

BS2VM_API int BGBDTC_EndLayoutClass(
	BGBDTC_ClassInfo *cls)
{
	BGBDTC_FinishLayoutClass(cls);
	return(0);
}

BS2VM_API BGBDTC_SlotInfo *BGBDTC_FetchClassSlotIndex(
	BGBDTC_ClassInfo *cls, int idx)
{
	if(!cls || (idx<0) || (idx>=cls->nslots))
		return(NULL);
	return(cls->slots[idx]);
}

int BGBDTC_CheckExpandClassSlotIndex(
	BGBDTC_ClassInfo *cls, int idx)
{
	int i;

	if(!cls->slots)
	{
		if(idx>0)
		{
			i=idx+1;
			if(i<4)i=4;
		}else
		{
			i=16;
//			while(i<=idx)
//				i=i+(i>>2);
		}
		cls->slots=dtmAlloc("bgbdtc_classslots_t",
			i*sizeof(BGBDTC_SlotInfo *));
		cls->mslots=i;
		return(1);
	}

	if(idx>=cls->mslots)
	{
		i=cls->mslots;
		while(i<=idx)
			i=i+(i>>1);
		cls->slots=dtmRealloc(cls->slots,
			i*sizeof(BGBDTC_SlotInfo *));
		cls->mslots=i;
		return(1);
	}
	
	return(0);
}

BS2VM_API BGBDTC_SlotInfo *BGBDTC_GetClassSlotIndex(
	BGBDTC_ClassInfo *cls, int idx)
{
	BGBDTC_SlotInfo *vi;

	if(!cls || (idx<0))
		return(NULL);
	
	if(idx>=cls->nslots)
	{
		BGBDTC_CheckExpandClassSlotIndex(cls, idx);
		cls->nslots=idx+1;
	}
	
	vi=cls->slots[idx];
	if(!vi)
	{
		vi=dtmAlloc("bgbdtc_slotinfo_t", sizeof(BGBDTC_SlotInfo));
		cls->slots[idx]=vi;
	}
	
	return(vi);
}

BS2VM_API BGBDTC_SlotInfo *BGBDTC_LookupClassSlotName(
	BGBDTC_ClassInfo *cls, char *name)
{
	BGBDTC_SlotInfo *vi;
	int i;

	if(!cls)
		return(NULL);

	for(i=0; i<cls->nslots; i++)
	{
		vi=cls->slots[i];
		if(vi->name && !strcmp(vi->name, name))
			return(vi);
	}
	return(NULL);
}

BS2VM_API BGBDTC_SlotInfo *BGBDTC_GetClassSlotName(
	BGBDTC_ClassInfo *cls, char *name)
{
	BGBDTC_SlotInfo *vi;

	if(!cls)
		return(NULL);

	vi=BGBDTC_LookupClassSlotName(cls, name);
	if(vi)return(vi);
	
	vi=BGBDTC_GetClassSlotIndex(cls, cls->nslots);
	vi->name=BGBDT_TagStr_StrSymbol(name);
	return(vi);
}

BS2VM_API BGBDTC_SlotInfo *BGBDTC_LookupClassSlotNameSig(
	BGBDTC_ClassInfo *cls, char *name, char *sig)
{
	BGBDTC_SlotInfo *vi;
	int i;

	if(!cls)
		return(NULL);

	for(i=0; i<cls->nslots; i++)
	{
		vi=cls->slots[i];
		if((name && vi->name && !strcmp(vi->name, name)) ||
			(!name && !vi->name))
		{
			if((sig && vi->sig && !strcmp(vi->sig, sig)) ||
				(!sig && !vi->sig))
			{
				return(vi);
			}
		}
	}
	return(NULL);
}

BS2VM_API BGBDTC_SlotInfo *BGBDTC_LookupClassMatchSlot(
	BGBDTC_ClassInfo *cls, BGBDTC_SlotInfo *svi)
{
	BGBDTC_SlotInfo *vi;
	int i;

	if(!cls || !svi)
		return(NULL);

	for(i=0; i<cls->nslots; i++)
	{
		vi=cls->slots[i];
		
		if(vi==svi)
			return(vi);
		
		if((svi->name && vi->name && !strcmp(vi->name, svi->name)) ||
			(svi->nameh==vi->nameh))
		{
			if((svi->sig && vi->sig && !strcmp(vi->sig, svi->sig)) ||
				(!svi->sig && !vi->sig))
			{
				return(vi);
			}
		}
	}
	
	if(cls->super)
	{
		vi=BGBDTC_LookupClassMatchSlot(cls->super, svi);
		return(vi);
	}
	
	return(NULL);
}

BS2VM_API dtcObject BGBDTC_AllocClassInstance(
	BGBDTC_ClassInfo *cls)
{
	void *ptr;
	
	ptr=dtmAlloc("bgbdtc_object_t", cls->szdata);
	*(void **)ptr=cls;
	return(ptr);
}

BS2VM_API dtcObject BGBDTC_AllocCloneInstance(
	dtcObject obj)
{
	BGBDTC_ClassInfo *cls;
	void *obj1;
	
	cls=*(void **)obj;
	obj1=BGBDTC_AllocClassInstance(cls);
	memcpy(obj1, obj, cls->szdata);
	return(obj1);
}

int BGBDTC_CheckExpandClassIfaceIndex(
	BGBDTC_ClassInfo *cls, int idx)
{
	int i;

	if(!cls->iface)
	{
		if(idx>0)
		{
			i=idx;
			if(i<4)i=4;
		}else
		{
			i=16;
//			while(i<=idx)
//				i=i+(i>>2);
		}
		cls->iface=dtmAlloc("bgbdtc_classiface_t",
			i*sizeof(BGBDTC_ClassInfo *));
		cls->miface=i;
		return(1);
	}

	if(idx>=cls->miface)
	{
		i=cls->miface;
		while(i<=idx)
			i=i+(i>>1);
		cls->iface=dtmRealloc(cls->iface,
			i*sizeof(BGBDTC_ClassInfo *));
		cls->miface=i;
		return(1);
	}
	
	return(0);
}

BS2VM_API int BGBDTC_CheckClassExtends(
	BGBDTC_ClassInfo *clsa, BGBDTC_ClassInfo *clsb)
{
	BGBDTC_ClassInfo *clsc;
	
	clsc=clsa;
	while(clsc)
	{
		if(clsc==clsb)
			return(1);
		clsc=clsc->super;
	}
	return(0);
}

BS2VM_API int BGBDTC_CheckClassImplements(
	BGBDTC_ClassInfo *clsa, BGBDTC_ClassInfo *clsb)
{
	BGBDTC_ClassInfo *clsc;
	int i;
	
	clsc=clsa;
	while(clsc)
	{
//		if(clsc==clsb)
//			return(1);
		for(i=0; i<clsc->niface; i++)
			if(clsc->iface[i]==clsb)
				return(1);
		clsc=clsc->super;
	}
	return(0);
}

BS2VM_API int BGBDTC_CheckObjInstanceof(
	dtcObject obj, BGBDTC_ClassInfo *cls)
{
	if(BGBDTC_CheckClassExtends(*(void **)obj, cls))
		return(1);
	if(BGBDTC_CheckClassImplements(*(void **)obj, cls))
		return(1);
	return(0);
}

BS2VM_API int BGBDTC_CheckObjvObjectP(dtVal obj)
{
	static int objty_clsobj=-1;
	void *ptr;
	
	if(objty_clsobj<0)
		{ objty_clsobj=BGBDT_MM_GetIndexObjTypeName("bgbdtc_object_t"); }
	return(dtvCheckPtrTagP(obj, objty_clsobj));
}

BS2VM_API int BGBDTC_CheckObjvInstanceof(
	dtVal obj, BGBDTC_ClassInfo *cls)
{
	static int objty_clsobj=-1;
	void *ptr;
	
	if(objty_clsobj<0)
		{ objty_clsobj=BGBDT_MM_GetIndexObjTypeName("bgbdtc_object_t"); }
	if(!dtvCheckPtrTagP(obj, objty_clsobj))
		return(0);

	ptr=dtvUnwrapPtrF(obj);

	if(BGBDTC_CheckObjInstanceof(ptr, cls))
		return(1);
	return(0);
}

#define DTC_CACHENAME_PFX(RV)							\
	dtcClass cls; dtcField fi; fi=*rfi;					\
	if(!fi)												\
	{	cls=BGBDTC_LookupClassQName(qn);				\
		fi=BGBDTC_LookupClassSlotName(cls, fn);			\
		if(!fi)return(RV);								\
		*rfi=fi;	}


BS2VM_API s32 BGBDTC_GetCacheNameI(dtcObject obj,
	dtcField *rfi, char *qn, char *fn)
{	DTC_CACHENAME_PFX(0)
	return(fi->GetI(obj, fi));	}
BS2VM_API s64 BGBDTC_GetCacheNameL(dtcObject obj,
	dtcField *rfi, char *qn, char *fn)
{	DTC_CACHENAME_PFX(0)
	return(fi->GetL(obj, fi));	}
BS2VM_API f32 BGBDTC_GetCacheNameF(dtcObject obj,
	dtcField *rfi, char *qn, char *fn)
{	DTC_CACHENAME_PFX(0.0)
	return(fi->GetF(obj, fi));	}
BS2VM_API f64 BGBDTC_GetCacheNameD(dtcObject obj,
	dtcField *rfi, char *qn, char *fn)
{	DTC_CACHENAME_PFX(0.0)
	return(fi->GetD(obj, fi));	}

BS2VM_API dtVal BGBDTC_GetCacheNameA(dtcObject obj,
	dtcField *rfi, char *qn, char *fn)
{	DTC_CACHENAME_PFX(DTV_UNDEFINED)
	return(fi->GetA(obj, fi));	}

BS2VM_API void *BGBDTC_GetCacheNamePtr(dtcObject obj,
	dtcField *rfi, char *qn, char *fn)
{
	DTC_CACHENAME_PFX(NULL)
	return(fi->GetPtr(obj, fi));
}


BS2VM_API int BGBDTC_SetCacheNameI(dtcObject obj,
	dtcField *rfi, char *qn, char *fn, s32 val)
{	DTC_CACHENAME_PFX(-1)
	fi->SetI(obj, fi, val);
	return(0); }
BS2VM_API int BGBDTC_SetCacheNameL(dtcObject obj,
	dtcField *rfi, char *qn, char *fn, s64 val)
{	DTC_CACHENAME_PFX(-1)
	fi->SetL(obj, fi, val);
	return(0); }
BS2VM_API int BGBDTC_SetCacheNameF(dtcObject obj,
	dtcField *rfi, char *qn, char *fn, f32 val)
{	DTC_CACHENAME_PFX(-1)
	fi->SetF(obj, fi, val);
	return(0); }
BS2VM_API int BGBDTC_SetCacheNameD(dtcObject obj,
	dtcField *rfi, char *qn, char *fn, f64 val)
{	DTC_CACHENAME_PFX(-1)
	fi->SetD(obj, fi, val);
	return(0); }
BS2VM_API int BGBDTC_SetCacheNameA(dtcObject obj,
	dtcField *rfi, char *qn, char *fn, dtVal val)
{	DTC_CACHENAME_PFX(-1)
	fi->SetA(obj, fi, val);
	return(0); }
