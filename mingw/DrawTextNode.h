//----------------------------------------------------------------------
// DrawTextNode.h
//----------------------------------------------------------------------
// 화면에 String을 출력하기 위한 정보이다.
//----------------------------------------------------------------------

#ifndef	__DRAWTEXTNODE_H__
#define	__DRAWTEXTNODE_H__

#include <Windows.h>

#define	FLAG_DRAWTEXT_OUTLINE			0x01	// 외곽선

class DRAWTEXT_NODE {
	public :
		DRAWTEXT_NODE(int x, int y, const char* str, COLORREF color=0x00FFFFFF, BYTE font=0, int flag=0)
			: String(str)
		{
			X		= x;
			Y		= y;
			Color	= color;
			Font	= font;
			Flag	= flag;	
			bBox	= false;
			bBoxOutline = false;
			TextTime = 0;
		}

		//-------------------------------------------------------
		// Box
		//-------------------------------------------------------
		bool			IsExistBox() const	{ return bBox; }
		void			SetBox(const RECT& rectBox, WORD boxColor, bool bTrans=true)
		{
			bBox		= true;
			Box			= rectBox;
			BoxColor	= boxColor;
			bTransBox	= bTrans;
		}
		RECT			GetBox() const		{ return Box; }
		WORD			GetBoxColor() const	{ return BoxColor; }

		bool			IsExistBoxOutline() const		{ return bBoxOutline; }
		void			SetBoxOutline(WORD color)
		{
			bBoxOutline = true;
			BoxOutlineColor = color;
		}
		
		WORD			GetBoxOutlineColor() const	{ return BoxOutlineColor; }

		//-------------------------------------------------------
		// Time
		//-------------------------------------------------------
		void			SetTextTime(QWORD time)	{ TextTime = time; }

		//-------------------------------------------------------
		// Get
		//-------------------------------------------------------
		int				GetX() const			{ return X; }		
		int				GetY() const			{ return Y; }
		const char*		GetString() const		{ return String; }
		COLORREF		GetColor() const		{ return Color; }
		BYTE			GetFont() const			{ return Font; }
		BYTE			IsOutline()	const		{ return Flag & FLAG_DRAWTEXT_OUTLINE; }
		bool			IsTransBox() const		{ return bTransBox; }
		QWORD			GetTextTime() const		{ return TextTime; }
		
	protected :
		int				X, Y;		// 화면 상의 출력 좌표
		const char*		String;		// 출력할려는 string
		COLORREF		Color;		// 글자 색
		BYTE			Font;		// font 종류
		BYTE			Flag;		// 글자 효과
		bool			bBox;		// box있나?
		RECT			Box;		// 글자 밑에 깔릴 Box영역
		WORD			BoxColor;	// box 색깔	
		bool			bBoxOutline;	// 박스에 테두리 있나?
		WORD			BoxOutlineColor;	// box선 색깔	
		bool			bTransBox;	// 투명한 박스인가?
		QWORD			TextTime;	// 출력 시간
};

#endif