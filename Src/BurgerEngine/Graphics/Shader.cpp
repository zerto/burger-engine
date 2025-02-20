#include "shader.h"
#include "shaderTool.h"

#include "BurgerEngine/Core/Engine.h"
#include "BurgerEngine/Graphics/RenderingContext.h"
#include "BurgerEngine/Core/AbstractCamera.h"
#include "BurgerEngine/Core/TimeContext.h"
#include "BurgerEngine/External/Math/Vector.h"

#include <iostream>

Shader::Shader()
{
	m_bIsReady = false;
}

bool Shader::LoadAndCompile(const std::string& sVert, const std::string& sFrag)
{
	GLhandleARB so[2]; 
	memset(so, 0, sizeof(GLhandleARB)*2);

	///Loading the shader
	so[0] = loadShader(sVert.c_str()); 
	if(so[0]==0){
		std::cerr << "loading shader "+sVert+" failed" << std::endl;
		return false;
	}

	///Compiling the shader, done by the graphic card
	if(!compileShader(so[0])){ 
		std::cerr << "compiling shader "+sVert+" failed" << std::endl;
		return false;
	}

	so[1] = loadShader(sFrag.c_str());
	if(so[1]==0){
		std::cerr << "loading shader "+sFrag+" failed " << std::endl;
		return false;
	}
	if(!compileShader(so[1])){
		std::cerr << "compiling shader "+sFrag+" failed " << std::endl;
		return false;
	}

	m_oProgram = linkShaders(so,2);
	m_bIsReady = true;
	return true;
}

void Shader::Activate()
{
	if (m_bIsReady)
		glUseProgramObjectARB(m_oProgram);
}

void Shader::Deactivate()
{
	glUseProgramObjectARB(0);
}

void Shader::setUniformf(const std::string& sName, float fValue)
{
	/// Need to Activate the shader before, but we are not doing it
	/// in here to save computation time (if we are setting several variable at the same time)
	glUniform1f(glGetUniformLocation(m_oProgram, sName.c_str()), fValue);
}

void Shader::setUniformf( int iUniformLocation, float fValue)
{
	/// Need to Activate the shader before, but we are not doing it
	/// in here to save computation time (if we are setting several variable at the same time)
	glUniform1f( iUniformLocation, fValue);
}

void Shader::setUniform2fv( int iUniformLocation, int iCount, float * pValue)
{
	/// Need to Activate the shader before, but we are not doing it
	/// in here to save computation time (if we are setting several variable at the same time)
	glUniform2fv( iUniformLocation, iCount, pValue);
}

void Shader::setUniform3fv( int iUniformLocation, int iCount, float * pValue)
{
	/// Need to Activate the shader before, but we are not doing it
	/// in here to save computation time (if we are setting several variable at the same time)
	glUniform3fv( iUniformLocation, iCount, pValue);
}

void Shader::setUniform4fv( int iUniformLocation, int iCount, float * pValue)
{
	/// Need to Activate the shader before, but we are not doing it
	/// in here to save computation time (if we are setting several variable at the same time)
	glUniform4fv( iUniformLocation, iCount, pValue);
}

void Shader::setUniformi(const std::string& sName, int fValue)
{
	/// Need to Activate the shader before, but we are not doing it
	/// in here to save computation time (if we are setting several variable at the same time)
	glUniform1i(glGetUniformLocation(m_oProgram, sName.c_str()), fValue);
}

void Shader::setUniformi( int iUniformLocation, int iValue)
{
	/// Need to Activate the shader before, but we are not doing it
	/// in here to save computation time (if we are setting several variable at the same time)
	glUniform1i( iUniformLocation, iValue);
}

void Shader::setUniformTexture(const std::string& sName, int iUnit)
{
	glUniform1iARB(glGetUniformLocationARB(m_oProgram, sName.c_str()), iUnit);
}

void Shader::setUniformTexture( int iUniformLocation, int iUnit)
{
	glUniform1iARB( iUniformLocation, iUnit);
}

void Shader::setUniformMatrix4fv(const std::string& sName, float * pValue)
{
	glUniformMatrix4fvARB(glGetUniformLocationARB(m_oProgram,sName.c_str()),1,GL_FALSE,pValue);
}

void Shader::setUniformMatrix4fv( int iUniformLocation, float * pValue)
{
	glUniformMatrix4fvARB( iUniformLocation,1,GL_FALSE,pValue);
}

void Shader::setUniformMatrix4fv( int iUniformLocation, unsigned int iCount, float * pValue)
{
	glUniformMatrix4fvARB( iUniformLocation,iCount,GL_FALSE,pValue);
}

GLhandleARB	Shader::getHandle()
{
	return m_oProgram;
}	

void Shader::QueryStdUniforms()
{
	m_oStdUniformsMap[ E_STD_INV_VIEWPORT ] = glGetUniformLocation( m_oProgram, "vInvViewport" );
	m_oStdUniformsMap[ E_STD_DOF_PARAMS ] = glGetUniformLocation( m_oProgram, "vDofParams" );
	m_oStdUniformsMap[ E_STD_MVP ] = glGetUniformLocation( m_oProgram, "mMVP" );
	m_oStdUniformsMap[ E_STD_INV_MVP ] = glGetUniformLocation( m_oProgram, "mInvMVP" );
	m_oStdUniformsMap[ E_STD_MODEL_VIEW ] = glGetUniformLocation( m_oProgram, "mModelView" );
	m_oStdUniformsMap[ E_STD_NORMAL_MATRIX ] = glGetUniformLocation( m_oProgram, "mNormalMatrix" );
	//m_oStdUniformsMap[ E_STD_INV_PROJECTION ] = glGetUniformLocation( m_oProgram, "vInProj" );
	m_oStdUniformsMap[ E_STD_TIME ] = glGetUniformLocation( m_oProgram, "fTime" );
}

void Shader::CommitStdUniforms()
{
	Engine & rEngine = Engine::GrabInstance();
	RenderingContext& rRenderContext = rEngine.GrabRenderContext();
	
	int iHandle;
	
	iHandle = m_oStdUniformsMap[ E_STD_INV_VIEWPORT ];
	if( iHandle != -1 )
	{
		vec2 vViewPort = vec2( 1.0f / (float)rEngine.GetWindowWidth(), 1.0f / (float)rEngine.GetWindowHeight() );
		glUniform2fv( iHandle, 1, (float*)vViewPort );
	}

	iHandle = m_oStdUniformsMap[ E_STD_DOF_PARAMS ];
	if( iHandle != -1 )
	{
		vec4 vDofParams = rEngine.GetCurrentCamera().GetDofParams();
		glUniform4fv( iHandle, 1, (float*)vDofParams );
	}

	iHandle = m_oStdUniformsMap[ E_STD_MVP ];
	if( iHandle != -1 )
	{
		glUniformMatrix4fv( iHandle, 1, true, (float*)rRenderContext.GetMVP() );
	}

	iHandle = m_oStdUniformsMap[ E_STD_INV_MVP ];
	if( iHandle != -1 )
	{
		unsigned int iWindowWidth = rEngine.GetWindowWidth();
		unsigned int iWindowHeight = rEngine.GetWindowHeight();

		AbstractCamera & rCamera = rEngine.GetCurrentCamera();
		float4x4 mProjection = GlperspectiveMatrix( rCamera.GetFOV(), (float)iWindowWidth/(float)iWindowHeight,rCamera.GetNear(), rCamera.GetFar() );

		float4x4 mView =  rCamera.GetViewMatrix();

		mat4 m4InvMVP = !(transpose(mProjection)*(mView)) ;
		//mat4 m4MVP = mProjection * ;
		glUniformMatrix4fv( iHandle, 1, false, (float*)m4InvMVP );
	}

	iHandle = m_oStdUniformsMap[ E_STD_NORMAL_MATRIX ];
	if( iHandle != -1 )
	{
		glUniformMatrix4fv( iHandle, 1, true, (float*)rRenderContext.GetNormalMatrix() );
	}

	iHandle = m_oStdUniformsMap[ E_STD_MODEL_VIEW ];
	if( iHandle != -1 )
	{
		glUniformMatrix4fv( iHandle, 1, true, (float*)rRenderContext.GetModelView() );
	}

	iHandle = m_oStdUniformsMap[ E_STD_TIME ];
	if( iHandle != -1 )
	{
		float fTime = rEngine.GetTimeContext().GetElapsedTime();
		glUniform1f( iHandle, fTime );
	}

}