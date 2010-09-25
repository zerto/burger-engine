/*************************************
*
*		BurgerEngine Project
*		
*		Created :	05/07/10
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/

#ifndef __DEFERREDRENDERER_H__
#define __DEFERREDRENDERER_H__

/// \name	DeferredRenderer.h
/// \brief	The application of the window

#include "CommonGraphics.h"
#include <vector>

class FBO;
class Shader;

class DeferredRenderer
{
public:

	/// \brief default constructor
	DeferredRenderer();
	~DeferredRenderer();

	void SetDebugFlag( int i){ m_iDebugFlag = i; }

	/// \brief strats rendering process
	void Render();

private:
	/// \brief Display a full screen quad
	/// this is temporary, we need a fullscreenquad class using VBO
	void DrawScreenSpaceQuad(int iWindowWidth, int iWindowHeight);

private:
	FBO* m_oGBuffer;
	FBO* m_oLightBuffer;

	int m_iDebugFlag;

	/*FBO * m_oFBOColorDepth;
	FBO * m_oFBOWriteAlpha;
	FBO * m_oFBOBlurH;
	FBO * m_oFBOBlurV;
	FBO * m_oFinalFBO;
	FBO * m_oPostProcessFBO0;
	FBO * m_oPostProcessFBO1;*/

	Shader* m_pOmniLightShader;

	//constants for lights (might be temporary...)
	GLfloat * m_pLightZeros;
	GLfloat * m_pLightDir;
};

#endif //__DEFERREDRENDERER_H__