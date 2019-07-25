/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#import <React/ARTShapeManager.h>

#import <React/ARTShape.h>
#import "RCTConvert+ART.h"

@implementation ARTShapeManager

RCT_EXPORT_MODULE()

- (ARTRenderable *)node
{
  return [ARTShape new];
}

RCT_EXPORT_VIEW_PROPERTY(d, CGPath)

@end
