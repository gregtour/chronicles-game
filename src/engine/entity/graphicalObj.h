#ifndef _GRAPHICALOBJ_H
#define _GRAPHICALOBJ_H

#include "../graphics/graphics.h"

class IGraphicalObj
{
public:
	IGraphicalObj();
	virtual ~IGraphicalObj();

	virtual void Render() = 0;
};

class IAlphaGraphicalObj
{
public:
	IAlphaGraphicalObj();
	virtual ~IAlphaGraphicalObj();

	virtual void RenderAlpha() = 0;
};

class IOrthoGraphicalObj
{
public:
	IOrthoGraphicalObj();
	virtual ~IOrthoGraphicalObj();

	virtual void RenderOrtho() = 0;
};

#endif
