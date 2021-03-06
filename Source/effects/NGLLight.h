/*
 *	Copyright (c) 2011-2015 NinevehGL. More information at: http://nineveh.gl
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the Software is
 *	furnished to do so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in
 *	all copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *	THE SOFTWARE.
 */

#import "NGLRuntime.h"
#import "NGLGlobal.h"
#import "NGLDataType.h"
#import "NGLObject3D.h"

/*!
 *					Represents the light type.
 *
 *					NinevehGL supports three kind of lights:
 *
 *						- Point Light (also known as Omni Light);
 *						- Spot Light;
 *						- Sky Light.
 *
 *					Each light has its own emission behavior, the light beans emission can change
 *					drastically based on light type.
 *
 *	@see			NGLLight::type
 *	
 *	@var			NGLLightTypePoint
 *					Represents a Point Light.
 *	
 *	@var			NGLLightTypeSpot
 *					Represents a Spot Light.
 *	
 *	@var			NGLLightTypeSky
 *					Represents a Sky Light.
 */
typedef enum
{
	NGLLightTypePoint,
	NGLLightTypeSpot,
	NGLLightTypeSky,
} NGLLightType;

/*!
 *					<strong>(Internal only)</strong> An object that holds the light scalar values.
 *
 *					This structure is used as a fixed pointer to preserve the necessary information (memory)
 *					to the light's scalar values.
 *
 *	@see			NGLLightType
 *	@see			NGLvec4
 *	
 *	@var			NGLLightValues::type
 *					The light type.
 *	
 *	@var			NGLLightValues::position
 *					The light world position vector.
 *	
 *	@var			NGLLightValues::color
 *					The light color.
 *	
 *	@var			NGLLightValues::attenuation
 *					The attenuation factor.
 */
typedef struct
{
	NGLLightType	type;
	NGLvec4			position;
	NGLvec4			color;
	float			attenuation;
} NGLLightValues;

/*!
 *					Creates and manages a light source. (Singleton)
 *
 *					NGLLight is a class to deal with light effects. It is directly connected with
 *					the NinevehGL Shader API and can produce all light effects. However, for performance
 *					reasons, it's always better to work with precalculated light on the diffuse and ambient
 *					maps instead of the real time light calculations.
 *
 *					In NinevehGL lights are calculated by two ways:
 *
 *						- Using the half vector;
 *							<pre>
 *							            \ V      H || N        / L
 *							              \        ||        /
 *							                \      ||      /
 *							                  \    ||    /
 *							                    \  ||  /
 *							______________________\||/__________________
 *							
 *							</pre>
 *						- Using the reflection vector;
 *							<pre>
 *							            \ V   \ R   | N        / L
 *							              \    \    |        /
 *							                \   \   |      /
 *							                  \  \  |    /
 *							                    \ \ |  /
 *							______________________\\|/__________________
 *							
 *							</pre>
 *
 *					Both approaches produce great results. But using the reflection vector produces more
 *					accurated results and for some situations it's essential, like for the bump map effect.
 *					The "Half Vector" is a vector lying exactly halfway between the V (View vector) and L
 *					(Light vector), it needs less calculation and is faster than the other approach.
 *
 *					On the other hand, the Reflection vector must be extract from a map like the bump map
 *					or a reflection map.
 */
@interface NGLLight : NGLObject3D
{
@private
	NGLLightValues			_values;
}

/*!
 *					The light type.
 *
 *	@see			NGLLightType
 */
@property (nonatomic) NGLLightType type;

/*!
 *					The light's color.
 *					The default color is white.
 */
@property (nonatomic) NGLvec4 color;

/*!
 *					The attenuation factor. This floating values lies on a range from (0.001, 1000.0).
 *					The attenuation factor represents the depth necessary for the light lose around 5% of
 *					its power. For example, attenuation factor of 2.0 means that an object distant from
 *					the light 20.0 units will receive 50% of the light power.
 *
 *					<pre>
 *					Attenuation: 2.0;
 *					Distance from Light: 20.0;
 *					                                                          
 *					  Light                                                    Object
 *					 + -----------------------------------------------------  o
 *					  |_________||_________||_________||_________||_________| 
 *
 *					100%        90%        80%        70%        60%       50%
 *
 *					</pre>
 *
 *					The default attenuation value is 1.0.
 */
@property (nonatomic) float	attenuation;

/*!
 *					<strong>(Internal only)</strong> Returns a pointer to the light scalar values.
 *					You should not call this method directly.
 */
@property (nonatomic, readonly) NGLLightValues *values;

/*!
 *					Returns the singleton instance of NGLLight.
 *
 *					For performance reasons, NinevehGL makes use of only one main light, which is a
 *					sky light. By default, the main light is positioned at the {0.0, 1.0, -1.0} in world
 *					coordinates.
 *
 *	@result			The singleton instance of the main Light.
 */
+ (NGLLight *) defaultLight;

@end