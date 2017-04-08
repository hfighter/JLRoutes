/*
 Copyright (c) 2017, Joel Levin
 All rights reserved.

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 Neither the name of JLRoutes nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN


@class JLRRouteDefinition;


/// The matching route pattern, passed in the handler parameters.
extern NSString *const JLRoutePatternKey;

/// The original URL that was routed, passed in the handler parameters.
extern NSString *const JLRouteURLKey;

/// The matching route scheme, passed in the handler parameters.
extern NSString *const JLRouteSchemeKey;

/// The wildcard components (if present) of the matching route, passed in the handler parameters.
extern NSString *const JLRouteWildcardComponentsKey;

/// The global routes namespace.
/// @see JLRoutes +globalRoutes
extern NSString *const JLRoutesGlobalRoutesScheme;


/// The JLRoutes class is the main entry-point into the JLRoutes framework.
/// Used for accessing schemes, managing routes, and routing URLs.

@interface JLRoutes : NSObject

/// Controls whether or not this router will try to match a URL with global routes if it can't be matched in the current namespace. Default is NO.
@property (nonatomic, assign) BOOL shouldFallbackToGlobalRoutes;

/// Called any time routeURL returns NO. Respects shouldFallbackToGlobalRoutes.
@property (nonatomic, copy, nullable) void (^unmatchedURLHandler)(JLRoutes *routes, NSURL *__nullable URL, NSDictionary<NSString *, id> *__nullable parameters);


///-------------------------------
/// @name Routing Schemes
///-------------------------------

#pragma mark - Routing Schemes

/// Returns the global routing scheme
+ (instancetype)globalRoutes;

/// Returns a routing namespace for the given scheme
+ (instancetype)routesForScheme:(NSString *)scheme;

/// Unregister and delete an entire scheme namespace
+ (void)unregisterRouteScheme:(NSString *)scheme;

/// Unregister all routes
+ (void)unregisterAllRouteSchemes;


///-------------------------------
/// @name Managing Routes
///-------------------------------

#pragma mark - Managing Routes

/// Add a route by directly inserted the route definition. This may be a subclass of JLRRouteDefinition to provide customized routing logic.
- (void)addRoute:(JLRRouteDefinition *)routeDefinition;

/// Registers a routePattern with default priority (0) in the receiving scheme namespace.
- (void)addRoute:(NSString *)routePattern handler:(BOOL (^__nullable)(NSDictionary<NSString *, id> *parameters))handlerBlock;

/// Registers a routePattern in the global scheme namespace with a handlerBlock to call when the route pattern is matched by a URL.
/// The block returns a BOOL representing if the handlerBlock actually handled the route or not. If
/// a block returns NO, JLRoutes will continue trying to find a matching route.
- (void)addRoute:(NSString *)routePattern priority:(NSUInteger)priority handler:(BOOL (^__nullable)(NSDictionary<NSString *, id> *parameters))handlerBlock;

/// Registers multiple routePatterns for one handler with default priority (0) in the receiving scheme namespace.
- (void)addRoutes:(NSArray<NSString *> *)routePatterns handler:(BOOL (^__nullable)(NSDictionary<NSString *, id> *parameters))handlerBlock;

/// Removes a routePattern from the receiving scheme namespace.
- (void)removeRoute:(NSString *)routePattern;

/// Removes all routes from the receiving scheme namespace.
- (void)removeAllRoutes;

/// Registers a routePattern with default priority (0) using dictionary-style subscripting.
- (void)setObject:(nullable id)handlerBlock forKeyedSubscript:(NSString *)routePatten;

/// Return all registered routes in the receiving scheme namespace.
- (NSArray <JLRRouteDefinition *> *)routes;

/// All registered routes, keyed by scheme
+ (NSDictionary <NSString *, NSArray <JLRRouteDefinition *> *> *)allRoutes;


///-------------------------------
/// @name Routing URLs
///-------------------------------

#pragma mark - Routing URLs

/// Returns whether a route will match a given URL in any routes scheme, but does not call any blocks.
+ (BOOL)canRouteURL:(nullable NSURL *)URL;

/// Returns whether a route will match a given URL in a specific scheme, but does not call any blocks.
- (BOOL)canRouteURL:(nullable NSURL *)URL;

/// Routes a URL in any routes scheme, calling handler blocks for patterns that match the URL until one returns YES.
/// If no matching route is found, the unmatchedURLHandler will be called (if set).
+ (BOOL)routeURL:(nullable NSURL *)URL;

/// Routes a URL in a specific scheme, calling handler blocks for patterns that match the URL until one returns YES.
/// If no matching route is found, the unmatchedURLHandler will be called (if set).
- (BOOL)routeURL:(nullable NSURL *)URL;

/// Routes a URL in any routes scheme, calling handler blocks (for patterns that match URL) until one returns YES.
/// Additional parameters get passed through to the matched route block.
+ (BOOL)routeURL:(nullable NSURL *)URL withParameters:(nullable NSDictionary<NSString *, id> *)parameters;

/// Routes a URL in a specific scheme, calling handler blocks (for patterns that match URL) until one returns YES.
/// Additional parameters get passed through to the matched route block.
- (BOOL)routeURL:(nullable NSURL *)URL withParameters:(nullable NSDictionary<NSString *, id> *)parameters;

@end


/// Global settings to use for parsing and routing.

@interface JLRoutes (GlobalOptions)

///----------------------------------
/// @name Configuring Global Options
///----------------------------------

#pragma mark - Configuring Global Options

/// Configures verbose logging. Defaults to NO.
+ (void)setVerboseLoggingEnabled:(BOOL)loggingEnabled;

/// Returns current verbose logging enabled state. Defaults to NO.
+ (BOOL)isVerboseLoggingEnabled;

/// Configures if '+' should be replaced with spaces in parsed values. Defaults to YES.
+ (void)setShouldDecodePlusSymbols:(BOOL)shouldDecode;

/// Returns if '+' should be replaced with spaces in parsed values. Defaults to YES.
+ (BOOL)shouldDecodePlusSymbols;

/// Configures if URL host is always considered to be a path component. Defaults to NO.
+ (void)setAlwaysTreatsHostAsPathComponent:(BOOL)treatsHostAsPathComponent;

/// Returns if URL host is always considered to be a path component. Defaults to NO.
+ (BOOL)alwaysTreatsHostAsPathComponent;

@end


#pragma mark - Deprecated

/// @see JLRoutePatternKey
extern NSString *const kJLRoutePatternKey               DEPRECATED_MSG_ATTRIBUTE("Use JLRoutePatternKey instead.");

/// @see JLRouteURLKey
extern NSString *const kJLRouteURLKey                   DEPRECATED_MSG_ATTRIBUTE("Use JLRouteURLKey instead.");

/// @see JLRouteSchemeKey
extern NSString *const kJLRouteSchemeKey                DEPRECATED_MSG_ATTRIBUTE("Use JLRouteSchemeKey instead.");

/// @see JLRouteWildcardComponentsKey
extern NSString *const kJLRouteWildcardComponentsKey    DEPRECATED_MSG_ATTRIBUTE("Use JLRouteWildcardComponentsKey instead.");

/// @see JLRoutesGlobalRoutesScheme
extern NSString *const kJLRoutesGlobalRoutesScheme      DEPRECATED_MSG_ATTRIBUTE("Use JLRoutesGlobalRoutesScheme instead.");

/// @see JLRouteSchemeKey
extern NSString *const kJLRouteNamespaceKey             DEPRECATED_MSG_ATTRIBUTE("Use JLRouteSchemeKey instead.");

/// @see JLRoutesGlobalRoutesScheme
extern NSString *const kJLRoutesGlobalNamespaceKey      DEPRECATED_MSG_ATTRIBUTE("Use JLRoutesGlobalRoutesScheme instead.");


/// Deprecated
///
/// All the class method conveniences listed here have been deprecated.
/// If you're using these, please switch to calling the matching instance method on +globalRoutes instead for the same behavior.

@interface JLRoutes (Deprecated)

+ (void)addRoute:(NSString *)routePattern handler:(BOOL (^__nullable)(NSDictionary<NSString *, id> *parameters))handlerBlock DEPRECATED_MSG_ATTRIBUTE("Use the matching instance method on +globalRoutes instead.");
+ (void)addRoute:(NSString *)routePattern priority:(NSUInteger)priority handler:(BOOL (^__nullable)(NSDictionary<NSString *, id> *parameters))handlerBlock DEPRECATED_MSG_ATTRIBUTE("Use the matching instance method on +globalRoutes instead.");
+ (void)addRoutes:(NSArray<NSString *> *)routePatterns handler:(BOOL (^__nullable)(NSDictionary<NSString *, id> *parameters))handlerBlock DEPRECATED_MSG_ATTRIBUTE("Use the matching instance method on +globalRoutes instead.");
+ (void)removeRoute:(NSString *)routePattern DEPRECATED_MSG_ATTRIBUTE("Use the matching instance method on +globalRoutes instead.");
+ (void)removeAllRoutes DEPRECATED_MSG_ATTRIBUTE("Use the matching instance method on +globalRoutes instead.");
+ (BOOL)canRouteURL:(nullable NSURL *)URL withParameters:(nullable NSDictionary<NSString *, id> *)parameters DEPRECATED_MSG_ATTRIBUTE("Use +canRouteURL: instead.");

// Other deprecations

- (BOOL)canRouteURL:(nullable NSURL *)URL withParameters:(nullable NSDictionary<NSString *, id> *)parameters DEPRECATED_MSG_ATTRIBUTE("Use -canRouteURL: instead.");

@end


NS_ASSUME_NONNULL_END
