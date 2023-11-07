#include "xgpwintext_render.h"

____BEGIN_SECTION____(gp)

bool GLWinFontRender::CreateBitmapFont(const char* fontname, unsigned int fontsize, DWORD fonttype)
{
	if (m_hdc == NULL) return false;

	if (m_hfont) DeleteObject(m_hfont); // delete old font

	// Create list all charactor vietnamese
	m_textbase = glGenLists(RANG_BASE_LIST);

	if (m_textbase == 0) return false;

	m_hfont = CreateFontA(fontsize, 0, 0, 0, fonttype, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
		FF_DONTCARE | DEFAULT_PITCH, fontname);

	// Verify font creation
	if (!m_hfont)
		return false;

	// Select a device context for the font
	SelectObject(m_hdc, m_hfont);

	if (!wglUseFontBitmapsW(m_hdc, 32, RANG_BASE_LIST, m_textbase))
	{
		DeleteObject(m_hfont);
		m_hfont = NULL;
	}

	return m_hfont ? true : false;
}


unsigned int GLWinFontRender::GetWchar(const wchar_t& ch)
{
	if (m_character_map_list.find(ch) != m_character_map_list.end())
	{
		return m_character_map_list[ch];
	}

	unsigned int id_list_char = glGenLists(1);
	SelectObject(m_hdc, m_hfont);

	BOOL rel = wglUseFontBitmaps(m_hdc, (DWORD)ch, 1, id_list_char);
	rel = (!rel) ? wglUseFontBitmapsW(m_hdc, (DWORD)ch, 1, id_list_char) : rel;

	if (!rel)
	{
		glDeleteLists(id_list_char, 1);
		return 0;
	}

	// Add list id to map character
	m_character_map_list[ch] = id_list_char;

	return id_list_char;
}

bool GLWinFontRender::LoadFont(const char* fontname, unsigned int fontsize, FontType fonttype /*= FontType::Normal*/)
{
	// Create font system
	DWORD sfonttype = FW_REGULAR;
	switch (fonttype)
	{
	case FontType::Bold:
		sfonttype = FW_BOLD;
		break;
	case FontType::Thin:
		sfonttype = FW_THIN;
		break;
	default:
		break;
	}

	m_bInitOK = CreateBitmapFont(fontname, fontsize, sfonttype);

	if (!m_bInitOK)
	{
		std::cerr << "Create bitmap font failed" << std::endl;
	}

	return m_bInitOK;
}

void GLWinFontRender::DontUse()
{
	if (!m_bInitOK) return;

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	// Restore matrices
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	glListBase(0);
	// Pop information matrix
	glPopAttrib();
}

void GLWinFontRender::Write(int x, int y, const char* text,  float r /*= 1.0f*/, float g /*= 1.0f*/, // Text out  [input]
															 float b /*= 1.0f*/, float a /*= 1.0f*/)
{
	if (!m_bInitOK || m_textbase == 0 || !text)
		return;

	glColor4f(r, g, b, a);
	glRasterPos2i(x, y);
	glCallLists((int)strlen(text), GL_UNSIGNED_BYTE, text);
}

void GLWinFontRender::WriteEach(int x, int y, const wchar_t* text, float r /*= 1.0f*/, float g /*= 1.0f*/, // Text out  [input]
																   float b /*= 1.0f*/, float a /*= 1.0f*/)
{
	if (!m_bInitOK || m_textbase == 0 || !text)
		return;

	glColor4f(r, g, b, a);
	glRasterPos2i(x, y);

	GLint id = 0;
	for (int i = 0; i < wcslen(text); i++)
	{
		if (short(text[i]) > RANG_BASE_LIST)
		{
			id = GetWchar(text[i]);
		}
		else
		{
			// 31 because 32 list not init + 1 list complie
			id = text[i] - 31;
		}

		if (id <= 0) continue;
		glCallList((GLuint)id);
	}
}

void GLWinFontRender::Write(int x, int y, const wchar_t* text, float r /*= 1.0f*/, float g /*= 1.0f*/, // Text out  [input]
															   float b /*= 1.0f*/, float a /* = 1.0f*/)
{
	if (!m_bInitOK || m_textbase == 0 || !text)
		return;

	glColor4f(r, g, b, a);
	glRasterPos2i(x, y);
	glCallLists((int)wcslen(text), GL_UNSIGNED_SHORT, text);
}

_____END_SECTION_____