/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#import <React/RCTSurfaceStage.h>
#import <React/RNPrimitives.h>

NS_ASSUME_NONNULL_BEGIN

@class RCTBridge;
@class RCTSurfaceView;
@class RNSurfacePresenter;
@protocol RCTSurfaceDelegate;

/**
 * (This is Fabric-compatible RCTSurface implementation.)
 *
 * RCTSurface instance represents React Native-powered piece of a user interface
 * which can be a full-screen app, separate modal view controller,
 * or even small widget.
 * It is called "Surface".
 *
 * The RCTSurface instance is completely thread-safe by design;
 * it can be created on any thread, and any its method can be called from
 * any thread (if the opposite is not mentioned explicitly).
 *
 * The primary goals of the RCTSurface are:
 *  * ability to measure and layout the surface in a thread-safe
 *    and synchronous manner;
 *  * ability to create a UIView instance on demand (later);
 *  * ability to communicate the current stage of the surface granularly.
 */
@interface RNFabricSurface : NSObject

@property (atomic, readonly) RCTSurfaceStage stage;
@property (atomic, readonly) NSString *moduleName;
@property (atomic, readonly) ReactTag rootTag;

@property (atomic, readwrite, weak, nullable) id<RCTSurfaceDelegate> delegate;

@property (atomic, copy, readwrite) NSDictionary *properties;

- (instancetype)initWithSurfacePresenter:(RNSurfacePresenter *)surfacePresenter
                              moduleName:(NSString *)moduleName
                       initialProperties:(NSDictionary *)initialProperties;

#pragma mark - Dealing with UIView representation, the Main thread only access

/**
 * Creates (if needed) and returns `UIView` instance which represents the Surface.
 * The Surface will cache and *retain* this object.
 * Returning the UIView instance does not mean that the Surface is ready
 * to execute and layout. It can be just a handler which Surface will use later
 * to mount the actual views.
 * RCTSurface does not control (or influence in any way) the size or origin
 * of this view. Some superview (or another owner) must use other methods
 * of this class to setup proper layout and interop interactions with UIKit
 * or another UI framework.
 * This method must be called only from the main queue.
 */
- (RCTSurfaceView *)view;

#pragma mark - Start & Stop

/**
 * Starts or stops the Surface.
 * A Surface object can be stopped and then restarted.
 * The starting process includes initializing all underlying React Native
 * infrastructure and running React app.
 * Just initialized Surface object starts automatically, there is no need
 * to call `start` explicitly. Surface also stops itself on deallocation
 * automatically.
 * Returns YES in case of success. Returns NO if the Surface is already
 * started or stopped.
 */
- (BOOL)start;
- (BOOL)stop;

#pragma mark - Layout: Setting the size constrains

/**
 * Sets `minimumSize` and `maximumSize` layout constraints for the Surface.
 */
- (void)setMinimumSize:(CGSize)minimumSize maximumSize:(CGSize)maximumSize;

/**
 * Previously set `minimumSize` layout constraint.
 * Defaults to `{0, 0}`.
 */
@property (atomic, assign, readonly) CGSize minimumSize;

/**
 * Previously set `maximumSize` layout constraint.
 * Defaults to `{CGFLOAT_MAX, CGFLOAT_MAX}`.
 */
@property (atomic, assign, readonly) CGSize maximumSize;

/**
 * Simple shortcut to `-[RCTSurface setMinimumSize:size maximumSize:size]`.
 */
- (void)setSize:(CGSize)size;

#pragma mark - Layout: Measuring

/**
 * Measures the Surface with given constraints.
 * This method does not cause any side effects on the surface object.
 */
- (CGSize)sizeThatFitsMinimumSize:(CGSize)minimumSize maximumSize:(CGSize)maximumSize;

/**
 * Return the current size of the root view based on (but not clamp by) current
 * size constraints.
 */
@property (atomic, assign, readonly) CGSize intrinsicSize;

#pragma mark - Synchronous waiting

/**
 * Synchronously blocks the current thread up to given `timeout` until
 * the Surface reaches `stage`.
 * NOT SUPPORTED IN FABRIC YET.
 */
- (BOOL)synchronouslyWaitForStage:(RCTSurfaceStage)stage timeout:(NSTimeInterval)timeout;

@end

@interface RNFabricSurface (Internal)

/**
 * Sets and clears given stage flags (bitmask).
 * Returns `YES` if the actual state was changed.
 */
- (BOOL)_setStage:(RCTSurfaceStage)stage;
- (BOOL)_unsetStage:(RCTSurfaceStage)stage;

@end

@interface RNFabricSurface (Deprecated)

/**
 * Deprecated. Use `initWithSurfacePresenter:moduleName:initialProperties` instead.
 */
- (instancetype)initWithBridge:(RCTBridge *)bridge
                    moduleName:(NSString *)moduleName
             initialProperties:(NSDictionary *)initialProperties;

/**
 * Deprecated. Use `rootTag` instead.
 */
@property (atomic, readonly) NSNumber *rootViewTag;

@end

NS_ASSUME_NONNULL_END
