#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cache/cache.h"

#include "vtim.h"
#include "vcc_xcounter_if.h"


static const char vsc_xcnt_name[] = "XCNT";

struct VSC_xcnt {
	uint64_t	val;
};

#define VSC_xcnt_size PRNDUP(sizeof(struct VSC_xcnt))


struct vmod_xcounter_vsc {
	unsigned		magic;
#define VMOD_XCOUNTER_VSC_PARAM_MAGIC		0xaed1b785
	struct vsc_seg	*vsc_seg;
	struct VSC_xcnt	*vsc;
	char            *json;
	VTAILQ_ENTRY(vmod_xcounter_vsc) list;
};

struct vsc_xcnt_seg {
	unsigned		magic;
#define VSC_XCNT_SEG_MAGIC		0xaed1b786
	struct vmod_xcounter_vsc	*vsc_vsc;
	VTAILQ_ENTRY(vsc_xcnt_seg) list;
};

struct vsc_xcnt_seg_head {
	unsigned magic;
#define VSC_XCNT_SEG_HEAD_MAGIC 0xaed1c73f
	VTAILQ_HEAD(, vsc_xcnt_seg) vsc_segs;
};


void
VSC_xcnt_New(struct vmod_xcounter_vsc *xcntvsc,  const char *format, const char* type, const char* level,  const char *oneliner,  const char *fmt, ...)
{
/*
	format: bitmap bytes duration integer
	type: bitmap counter gauge
	order: fixed(1000)
	level: info diag debug
*/
/*
	const unsigned char vsc_xcnt_json[] = "{"
	"  \"version\": \"1\","
	"  \"name\": \"xcnt\","
	"  \"oneliner\": \"Dynamic VSC\","
	"  \"order\": 20,"
	"  \"docs\": \"\","
	"  \"elements\": 1,"
	"  \"elem\": {"
	"    \"val\": {"
	"      \"name\": \"val\","
	"      \"format\": \"bytes\","
	"      \"oneliner\": \"Dynamic VSC\","
	"      \"type\": \"counter\","
	"      \"level\": \"info\","
	"      \"ctype\": \"uint64_t\","
	"      \"index\": 0,"
	"      \"docs\": \"Dynamic VSC\""
	"    }"
	"  }"
	"}";

*/
	
	va_list ap;
	
	char vsc_xcnt_json_fmt[] = "{"
	"  \"version\": \"1\","
	"  \"name\": \"xcnt\","
	"  \"oneliner\": \"\","
	"  \"order\": 1000,"
	"  \"docs\": \"\","
	"  \"elements\": 1,"
	"  \"elem\": {"
	"    \"val\": {"
	"      \"name\": \"val\","
	"      \"format\": \"%s\","
	"      \"oneliner\": \"%s\","
	"      \"type\": \"%s\","
	"      \"level\": \"%s\","
	"      \"ctype\": \"uint64_t\","
	"      \"index\": 0,"
	"      \"docs\": \"\""
	"    }"
	"  }"
	"}";
	
	size_t t;
	
	t = strlen(format);
	t+= strlen(oneliner);
	t+= strlen(type);
	t+= strlen(level);
	t+=sizeof vsc_xcnt_json_fmt;


	xcntvsc->json=calloc(t, 1);
	AN(xcntvsc->json);
	snprintf(xcntvsc->json, t, vsc_xcnt_json_fmt, format, oneliner, type, level);
	va_start(ap, fmt);
	xcntvsc->vsc = VRT_VSC_Alloc(NULL, &xcntvsc->vsc_seg, vsc_xcnt_name, VSC_xcnt_size,
		(const unsigned char*)xcntvsc->json, t, fmt, ap);
	va_end(ap);

}

void
VSC_xcnt_Destroy(struct vsc_seg **sg)
{
	struct vsc_seg *p;

	AN(sg);
	p = *sg;
	*sg = NULL;
	VRT_VSC_Destroy(vsc_xcnt_name, p);
}


static void
free_func(void *p)
{
	struct vsc_xcnt_seg_head *dsh;
	struct vsc_xcnt_seg  *ds;
	struct vsc_xcnt_seg  *ds2;
	CAST_OBJ_NOTNULL(dsh, p, VSC_XCNT_SEG_HEAD_MAGIC);
	VTAILQ_FOREACH(ds, &dsh->vsc_segs, list) {
		ds2 = ds;
		VTAILQ_REMOVE(&dsh->vsc_segs, ds, list);
		FREE_OBJ(ds2);
	}
	FREE_OBJ(dsh);
}


int v_matchproto_(vmod_event_f)
event_function(VRT_CTX, struct vmod_priv *priv, enum vcl_event_e e)
{
	struct vsc_xcnt_seg_head *dsh;
	struct vsc_xcnt_seg  *ds;
	switch (e) {
	case VCL_EVENT_LOAD:
		if (priv->priv == NULL) {
			ALLOC_OBJ(dsh, VSC_XCNT_SEG_HEAD_MAGIC);
			priv->priv = dsh;
			priv->free = free_func;
		}
		break;
	case VCL_EVENT_WARM:
		CAST_OBJ_NOTNULL(dsh, priv->priv, VSC_XCNT_SEG_HEAD_MAGIC);
		VTAILQ_FOREACH(ds, &dsh->vsc_segs, list) {
			VRT_VSC_Reveal(ds->vsc_vsc->vsc_seg);
		}
		
		break;
	case VCL_EVENT_COLD:
		CAST_OBJ_NOTNULL(dsh, priv->priv, VSC_XCNT_SEG_HEAD_MAGIC);
		VTAILQ_FOREACH(ds, &dsh->vsc_segs, list) {
			VRT_VSC_Hide(ds->vsc_vsc->vsc_seg);
		}
		break;
	case VCL_EVENT_DISCARD:
		return (0);
		break;
	default:
		return (0);
	}

	return (0);
}

VCL_VOID v_matchproto_()
vmod_vsc__init(VRT_CTX, struct vmod_xcounter_vsc **xcntvscp,
    const char *vcl_name, struct vmod_priv *priv, VCL_ENUM format, VCL_ENUM type, VCL_ENUM level, VCL_STRING oneliner, VCL_BOOL hidecold, VCL_BOOL hidevclname, VCL_STRING groupname)
{

	struct vmod_xcounter_vsc *xcntvsc;

	AN(xcntvscp);
	AZ(*xcntvscp);
	ALLOC_OBJ(xcntvsc, VMOD_XCOUNTER_VSC_PARAM_MAGIC);
	AN(xcntvsc);

	
	*xcntvscp = xcntvsc;
	if(hidevclname){
		VSC_xcnt_New(xcntvsc, format, type, level, oneliner, "%s%s", groupname, vcl_name);
	}else{
		VSC_xcnt_New(xcntvsc, format, type, level, oneliner, "%s.%s%s", VCL_Name(ctx->vcl), groupname, vcl_name);
	}

	struct vsc_xcnt_seg_head *dsh;
	struct vsc_xcnt_seg  *ds;
	CAST_OBJ_NOTNULL(dsh, priv->priv, VSC_XCNT_SEG_HEAD_MAGIC);
	ALLOC_OBJ(ds, VSC_XCNT_SEG_MAGIC);
	ds->vsc_vsc = xcntvsc;
	if(hidecold) VTAILQ_INSERT_HEAD(&dsh->vsc_segs, ds, list);

}

VCL_VOID v_matchproto_()
vmod_vsc__fini(struct vmod_xcounter_vsc **xcntvscp)
{
	struct vmod_xcounter_vsc *xcntvsc;

	if (*xcntvscp == NULL)
		return;

	TAKE_OBJ_NOTNULL(xcntvsc, xcntvscp, VMOD_XCOUNTER_VSC_PARAM_MAGIC);
	VSC_xcnt_Destroy(&xcntvsc->vsc_seg);
	if(xcntvsc->json) free(xcntvsc->json);
	FREE_OBJ(xcntvsc);
}


VCL_VOID v_matchproto_()
vmod_vsc_incr(VRT_CTX, struct vmod_xcounter_vsc *xcntvsc, VCL_INT d, VCL_BOOL threadsafe)
{
	if(d < 0) return;
	if(threadsafe){
		__sync_fetch_and_add(&xcntvsc->vsc->val, d);
	}else{
		xcntvsc->vsc->val += d;
	}
}

VCL_VOID v_matchproto_()
vmod_vsc_decr(VRT_CTX, struct vmod_xcounter_vsc *xcntvsc, VCL_INT d, VCL_BOOL threadsafe)
{
	if(d < 0) return;
	if(threadsafe){
		__sync_fetch_and_sub(&xcntvsc->vsc->val, d);
	}else{
		xcntvsc->vsc->val -= d;
	}
}

VCL_VOID v_matchproto_()
vmod_vsc_set(VRT_CTX, struct vmod_xcounter_vsc *xcntvsc, VCL_INT d)
{
	if(d < 0) return;
	xcntvsc->vsc->val= d;
}

VCL_INT v_matchproto_()
vmod_vsc_get(VRT_CTX, struct vmod_xcounter_vsc *xcntvsc)
{
	return(xcntvsc->vsc->val);
}
