// SPWindow_GaugeMap.h

#pragma warning (disable:4065)

#ifndef SPWindow_Gauge_h
#define SPWindow_Gauge_h

#include "SPWindow_Base.h"

class SPWindow_Gauge : public SPWindow_Base
{
public:
	SPWindow_Gauge();
	~SPWindow_Gauge();

	virtual void* Get(int ID)
	{
		switch(ID)
		{
		case GET_SPWINDOW_GAUGE_SET:					return &m_imagerc_Gauge;
		case GET_SPWINDOW_GAUGE_EDGE_SET:				return &m_imagerc_Edge;
		case GET_SPWINDOW_GAUGE_DECO_SET:				return &m_imagerc_Deco;
		case GET_SPWINDOW_GAUGE_FRAMEHEAD_SET:			return &m_imagerc_FrameHead_Gauge;
		case GET_SPWINDOW_GAUGE_FRAMEBODY_SET:			return &m_imagerc_FrameBody_Gauge;
		case GET_SPWINDOW_GAUGE_FRAMETAIL_SET:			return &m_imagerc_FrameTail_Gauge;
		case GET_SPWINDOW_GAUGE_FLICKER_SET:			return &m_imagerc_Flicker_Gauge;
		case GET_SPWINDOW_GAUGE_FLICKER_EDGE_SET:		return &m_imagerc_Flicker_Edge;
		case GET_SPWINDOW_GAUGE_FLICKER_DECO_SET:		return &m_imagerc_Flicker_Deco;
		default:
			return SPWindow_Base::Get(ID);
		}
	}

	virtual void Set(int ID,char* Value)
	{
		switch(ID)
		{
		case SET_SPWINDOW_GAUGE_FILENAME:					lstrcpy(m_imagerc_Gauge.FileName,Value);					break;
		case SET_SPWINDOW_GAUGE_X: 							m_imagerc_Gauge.x					= atoi(Value);	break;
		case SET_SPWINDOW_GAUGE_Y: 							m_imagerc_Gauge.y					= atoi(Value);	break;
		case SET_SPWINDOW_GAUGE_CX: 						m_imagerc_Gauge.cx					= atoi(Value);	break;
		case SET_SPWINDOW_GAUGE_CY: 						m_imagerc_Gauge.cy					= atoi(Value);	break;
		case SET_SPWINDOW_GAUGE_EDGE_FILENAME:	  			lstrcpy(m_imagerc_Edge.FileName,				 Value);					break;
		case SET_SPWINDOW_GAUGE_EDGE_X: 					m_imagerc_Edge.x					= atoi(Value);	break;
		case SET_SPWINDOW_GAUGE_EDGE_Y: 					m_imagerc_Edge.y					= atoi(Value);	break;
		case SET_SPWINDOW_GAUGE_EDGE_CX: 					m_imagerc_Edge.cx					= atoi(Value);	break;
		case SET_SPWINDOW_GAUGE_EDGE_CY: 					m_imagerc_Edge.cy					= atoi(Value);	break;
		case SET_SPWINDOW_GAUGE_DECO_FILENAME:	  			lstrcpy(m_imagerc_Deco.FileName				, Value);					break;
		case SET_SPWINDOW_GAUGE_DECO_X: 					m_imagerc_Deco.x					= atoi(Value);	break;
		case SET_SPWINDOW_GAUGE_DECO_Y: 					m_imagerc_Deco.y					= atoi(Value);	break;
		case SET_SPWINDOW_GAUGE_DECO_CX: 					m_imagerc_Deco.cx					= atoi(Value);	break;
		case SET_SPWINDOW_GAUGE_DECO_CY: 					m_imagerc_Deco.cy					= atoi(Value);	break;
		case SET_SPWINDOW_GAUGE_FRAMEHEAD_FILENAME:	  		lstrcpy(m_imagerc_FrameHead_Gauge.FileName	, Value);					break;
		case SET_SPWINDOW_GAUGE_FRAMEHEAD_X: 				m_imagerc_FrameHead_Gauge.x			= atoi(Value);	break;
		case SET_SPWINDOW_GAUGE_FRAMEHEAD_Y: 				m_imagerc_FrameHead_Gauge.y			= atoi(Value);	break;
		case SET_SPWINDOW_GAUGE_FRAMEHEAD_CX: 				m_imagerc_FrameHead_Gauge.cx		= atoi(Value);	break;
		case SET_SPWINDOW_GAUGE_FRAMEHEAD_CY: 				m_imagerc_FrameHead_Gauge.cy		= atoi(Value);	break;
		case SET_SPWINDOW_GAUGE_FRAMEBODY_FILENAME:	  		lstrcpy(m_imagerc_FrameBody_Gauge.FileName	, Value);					break;
		case SET_SPWINDOW_GAUGE_FRAMEBODY_X: 				m_imagerc_FrameBody_Gauge.x			= atoi(Value);	break;
		case SET_SPWINDOW_GAUGE_FRAMEBODY_Y: 				m_imagerc_FrameBody_Gauge.y			= atoi(Value);	break;
		case SET_SPWINDOW_GAUGE_FRAMEBODY_CX: 				m_imagerc_FrameBody_Gauge.cx		= atoi(Value);	break;
		case SET_SPWINDOW_GAUGE_FRAMEBODY_CY: 				m_imagerc_FrameBody_Gauge.cy		= atoi(Value);	break;
		case SET_SPWINDOW_GAUGE_FRAMETAIL_FILENAME:	  		lstrcpy(m_imagerc_FrameTail_Gauge.FileName	, Value);					break;
		case SET_SPWINDOW_GAUGE_FRAMETAIL_X: 				m_imagerc_FrameTail_Gauge.x			= atoi(Value);	break;
		case SET_SPWINDOW_GAUGE_FRAMETAIL_Y: 				m_imagerc_FrameTail_Gauge.y			= atoi(Value);	break;
		case SET_SPWINDOW_GAUGE_FRAMETAIL_CX: 				m_imagerc_FrameTail_Gauge.cx		= atoi(Value);	break;
		case SET_SPWINDOW_GAUGE_FRAMETAIL_CY: 				m_imagerc_FrameTail_Gauge.cy		= atoi(Value);	break;
		case SET_SPWINDOW_GAUGE_FLICKER_FILENAME:	  	    lstrcpy(m_imagerc_Flicker_Gauge.FileName	, Value);					break;
		case SET_SPWINDOW_GAUGE_FLICKER_X: 					m_imagerc_Flicker_Gauge.x			= atoi(Value);	break;
		case SET_SPWINDOW_GAUGE_FLICKER_Y: 					m_imagerc_Flicker_Gauge.y			= atoi(Value);	break;
		case SET_SPWINDOW_GAUGE_FLICKER_CX: 				m_imagerc_Flicker_Gauge.cx			= atoi(Value);	break;
		case SET_SPWINDOW_GAUGE_FLICKER_CY: 				m_imagerc_Flicker_Gauge.cy			= atoi(Value);	break;
		case SET_SPWINDOW_GAUGE_FLICKER_EDGE_FILENAME:	  	lstrcpy(m_imagerc_Flicker_Edge.FileName		, Value);					break;
		case SET_SPWINDOW_GAUGE_FLICKER_EDGE_X: 			m_imagerc_Flicker_Edge.x			= atoi(Value);	break;
		case SET_SPWINDOW_GAUGE_FLICKER_EDGE_Y: 			m_imagerc_Flicker_Edge.y			= atoi(Value);	break;
		case SET_SPWINDOW_GAUGE_FLICKER_EDGE_CX: 			m_imagerc_Flicker_Edge.cx			= atoi(Value);	break;
		case SET_SPWINDOW_GAUGE_FLICKER_EDGE_CY: 			m_imagerc_Flicker_Edge.cy			= atoi(Value);	break;
		case SET_SPWINDOW_GAUGE_FLICKER_DECO_FILENAME:	  	lstrcpy(m_imagerc_Flicker_Deco.FileName		, Value);					break;
		case SET_SPWINDOW_GAUGE_FLICKER_DECO_X: 			m_imagerc_Flicker_Deco.x			= atoi(Value);	break;
		case SET_SPWINDOW_GAUGE_FLICKER_DECO_Y: 			m_imagerc_Flicker_Deco.y			= atoi(Value);	break;
		case SET_SPWINDOW_GAUGE_FLICKER_DECO_CX: 			m_imagerc_Flicker_Deco.cx			= atoi(Value);	break;
		case SET_SPWINDOW_GAUGE_FLICKER_DECO_CY: 			m_imagerc_Flicker_Deco.cy			= atoi(Value);	break;
		default:
			SPWindow_Base::Set(ID,Value);
		}
	}

	s_ImageRect4 m_imagerc_Gauge,		     m_imagerc_Edge,          m_imagerc_Deco;
	s_ImageRect4 m_imagerc_FrameHead_Gauge;
	s_ImageRect4 m_imagerc_FrameBody_Gauge;
	s_ImageRect4 m_imagerc_FrameTail_Gauge;
	s_ImageRect4 m_imagerc_Flicker_Gauge,  m_imagerc_Flicker_Edge,  m_imagerc_Flicker_Deco;
protected:
private:
};


#endif
