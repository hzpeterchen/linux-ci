#undef TRACE_SYSTEM
#define TRACE_SYSTEM chipidea

#if !defined(_TRACE_CHIPIDEA_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_CHIPIDEA_H

#include <linux/tracepoint.h>
#include <linux/usb/gadget.h>
#include <linux/usb/ch9.h>

DECLARE_EVENT_CLASS(ci_event,
	TP_PROTO(struct ci13xxx *ci),

	TP_ARGS(ci),

	TP_STRUCT__entry(
		__field(	int,		id	)
	),

	TP_fast_assign(
		__entry->id = to_platform_device(ci->dev)->id;
	),

	TP_printk("ci_hdrc.%d", __entry->id)
);

DEFINE_EVENT(ci_event, ci_bus_reset,
	TP_PROTO(struct ci13xxx *ci),
	TP_ARGS(ci));
DEFINE_EVENT(ci_event, ci_int_uri,
	TP_PROTO(struct ci13xxx *ci),
	TP_ARGS(ci));
DEFINE_EVENT(ci_event, ci_int_ui,
	TP_PROTO(struct ci13xxx *ci),
	TP_ARGS(ci));
DEFINE_EVENT(ci_event, ci_int_pci,
	TP_PROTO(struct ci13xxx *ci),
	TP_ARGS(ci));
DEFINE_EVENT(ci_event, ci_int_sli,
	TP_PROTO(struct ci13xxx *ci),
	TP_ARGS(ci));

TRACE_EVENT(ci_ep_setup,
	TP_PROTO(struct ci13xxx_ep *cep, struct usb_ctrlrequest *creq),

	TP_ARGS(cep, creq),

	TP_STRUCT__entry(
		__field(	int,				id	)
		__field(	u8,				addr	)
		__field(	u8,				type	)
		__field(	u8,				req	)
		__field(	u16,				value	)
		__field(	u16,				index	)
		__field(	u16,				length	)
	),

	TP_fast_assign(
		__entry->id = to_platform_device(cep->ci->dev)->id;
		__entry->addr = ci_ep_addr(cep);
		__entry->type = creq->bRequestType;
		__entry->req = creq->bRequest;
		__entry->value = le16_to_cpu(creq->wValue);
		__entry->index = le16_to_cpu(creq->wIndex);
		__entry->length = le16_to_cpu(creq->wLength);
	),

	TP_printk("ci_hdrc.%d: [%02x] %02x %02x %04x %04x %d",
		  __entry->id, __entry->addr, __entry->type, __entry->req,
		  __entry->value, __entry->index, __entry->length)
);

TRACE_EVENT(ci_ep_complete_req,
	TP_PROTO(struct ci13xxx_ep *cep, u32 token, int status),

	TP_ARGS(cep, token, status),

	TP_STRUCT__entry(
		__field(	int,				id	)
		__field(	u8,				addr	)
		__field(	u32,				token	)
		__field(	int,				status	)
	),

	TP_fast_assign(
		__entry->id = to_platform_device(cep->ci->dev)->id;
		__entry->addr = ci_ep_addr(cep);
		__entry->token = token;
		__entry->status = status;
	),

	TP_printk("ci_hdrc.%d: [%02x] %08x status=%d",
		  __entry->id, __entry->addr, __entry->token, __entry->status)
);

DECLARE_EVENT_CLASS(ci_ep_event,
	TP_PROTO(struct ci13xxx_ep *cep, int status),

	TP_ARGS(cep, status),

	TP_STRUCT__entry(
		__field(	int,		id	)
		__field(	u8,		addr	)
		__field(	int,		status	)
	),

	TP_fast_assign(
		__entry->id = to_platform_device(cep->ci->dev)->id;
		__entry->addr = ci_ep_addr(cep);
		__entry->status = status;
	),

	TP_printk("ci_hdrc.%d: [%02x] status=%d", __entry->id,
		  __entry->addr, __entry->status)
);

DEFINE_EVENT(ci_ep_event, ci_ep_error,
	TP_PROTO(struct ci13xxx_ep *cep, int status),
	TP_ARGS(cep, status));
DEFINE_EVENT(ci_ep_event, ci_ep_queue_req,
	TP_PROTO(struct ci13xxx_ep *cep, int status),
	TP_ARGS(cep, status));
DEFINE_EVENT(ci_ep_event, ci_ep_dequeue_req,
	TP_PROTO(struct ci13xxx_ep *cep, int status),
	TP_ARGS(cep, status));
DEFINE_EVENT(ci_ep_event, ci_ep_enable,
	TP_PROTO(struct ci13xxx_ep *cep, int status),
	TP_ARGS(cep, status));
DEFINE_EVENT(ci_ep_event, ci_ep_disable,
	TP_PROTO(struct ci13xxx_ep *cep, int status),
	TP_ARGS(cep, status));
DEFINE_EVENT(ci_ep_event, ci_ep_alloc_req,
	TP_PROTO(struct ci13xxx_ep *cep, int status),
	TP_ARGS(cep, status));
DEFINE_EVENT(ci_ep_event, ci_ep_free_req,
	TP_PROTO(struct ci13xxx_ep *cep, int status),
	TP_ARGS(cep, status));
DEFINE_EVENT(ci_ep_event, ci_ep_halt,
	TP_PROTO(struct ci13xxx_ep *cep, int status),
	TP_ARGS(cep, status));
DEFINE_EVENT(ci_ep_event, ci_ep_wedge,
	TP_PROTO(struct ci13xxx_ep *cep, int status),
	TP_ARGS(cep, status));
DEFINE_EVENT(ci_ep_event, ci_ep_flush,
	TP_PROTO(struct ci13xxx_ep *cep, int status),
	TP_ARGS(cep, status));

#endif /* _TRACE_CHIPIDEA_H */

/* This part must be outside protection */
#include <trace/define_trace.h>
