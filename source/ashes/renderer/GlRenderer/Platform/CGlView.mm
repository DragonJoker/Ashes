#import "CGlView.hh"

#import <Cocoa/Cocoa.h>

GLView makeGLView( const void * handle )
{
	NSView * contentView = static_cast< NSView * >( handle );
	assert([contentView isKindOfClass:[NSView class]]);

	NSOpenGLPixelFormatAttribute attributes[] =
	{
		NSOpenGLPFADoubleBuffer,
		NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion4_1Core,
		NSOpenGLPFAColorSize, 24,
		NSOpenGLPFAAlphaSize, 8,
		NSOpenGLPFADepthSize, 24,
		NSOpenGLPFAStencilSize, 8,
		NSOpenGLPFASampleBuffers, 0,
		0,
	};
	NSOpenGLPixelFormat * pixelFormat = [[[NSOpenGLPixelFormat alloc] initWithAttributes:attributes] autorelease];
	NSOpenGLView * openGLView = [[[NSOpenGLView alloc] initWithFrame:[[[contentView window] contentView] bounds] pixelFormat:pixelFormat] autorelease];
	[openGLView  setWantsBestResolutionOpenGLSurface:NO];
	openGLView.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
	[contentView addSubview:openGLView];
	[openGLView prepareOpenGL];
	[openGLView update];
	return GLView
	{
		openGLView,
		[[openGLView openGLContext] CGLContextObj],
	};
}

void freeGLView( GLView view )
{
	NSOpenGLView * openGLView = static_cast< NSOpenGLView * >( view.view );
	assert([openGLView isKindOfClass:[NSOpenGLView class]]);
	[openGLView removeFromSuperview];
}
