module FragmentMap = ApolloClient__Utilities_Graphql_Fragments.FragmentMap;
module SelectionSetNode = ApolloClient__Graphql_Language_Ast.SelectionSetNode;

module KeyFieldsContext = {
  module Js_ = {
    // declare type KeyFieldsContext = {
    //     typename?: string;
    //     selectionSet?: SelectionSetNode;
    //     fragmentMap?: FragmentMap;
    //     keyObject?: Record<string, any>;
    // };
    type t = {
      typename: option(string),
      selectionSet: option(SelectionSetNode.t),
      fragment: option(FragmentMap.t),
      keyObject: option(Js.Json.t),
    };
  };

  type t = Js_.t;
};

module KeyFieldsFunction = {
  module Js_ = {
    // export declare type KeyFieldsFunction = (object: Readonly<StoreObject>, context: KeyFieldsContext) => KeySpecifier | ReturnType<IdGetter>;
    type t = (Js.Json.t, KeyFieldsContext.t) => string;
  };

  type t = Js_.t;
};

// export declare const defaultDataIdFromObject: KeyFieldsFunction;
[@bs.module "@apollo/client"]
external defaultDataIdFromObject: KeyFieldsFunction.t =
  "defaultDataIdFromObject";

module TypePolicy = {
  module Js_ = {
    // export declare type TypePolicy = {
    //     keyFields?: KeySpecifier | KeyFieldsFunction | false;
    //     queryType?: true;
    //     mutationType?: true;
    //     subscriptionType?: true;
    //     fields?: {
    //         [fieldName: string]: FieldPolicy<any> | FieldReadFunction<any>;
    //     };
    // };
    type t;
  };
  type t = Js_.t;
};

module TypePolicies = {
  module Js_ = {
    // export declare type TypePolicies = {
    //     [__typename: string]: TypePolicy;
    // };
    type t = Js.Dict.t(TypePolicy.Js_.t);
  };
  type t = Js_.t;
};

module PossibleTypesMap = {
  module Js_ = {
    // export declare type PossibleTypesMap = {
    //     [supertype: string]: string[];
    // };
    type t = Js.Dict.t(array(string));
  };

  type t = Js_.t;
};
