module FieldNode = ApolloClient__Graphql_Language_Ast.FieldNode;
module FragmentMap = ApolloClient__Utilities_Graphql_Fragments.FragmentMap;

module FragmentMatcher = {
  module Js_ = {
    // export declare type FragmentMatcher = (rootValue: any, typeCondition: string, context: any) => boolean;
    type t = (Js.Json.t, string, Js.Json.t) => bool;
  };

  type t = Js_.t;
};

// TODO: resolve dependency cycle
type apolloClient;

module Resolver = {
  module Js_ = {
    type t_context = {
      client: apolloClient,
      cache: ApolloClient__Cache_Core_Cache.ApolloCache.t(Js.Json.t),
    };

    type t_info = {
      field: FieldNode.t,
      fragmentMap: FragmentMap.t,
    };

    // export declare type Resolver = (rootValue?: any, args?: any, context?: any, info?: {
    //     field: FieldNode;
    //     fragmentMap: FragmentMap;
    // }) => any;
    type t = (Js.Json.t, Js.Json.t, t_context, t_info) => Js.Json.t;
  };

  type t = Js_.t;
};
