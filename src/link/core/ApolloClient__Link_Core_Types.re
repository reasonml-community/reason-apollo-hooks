module Graphql = ApolloClient__Graphql;
module GraphQLError = ApolloClient__Graphql_Error_GraphQLError;
module Observable = ApolloClient__ZenObservable.Observable;

module GraphQLRequest = {
  module Js_ = {
    // export interface GraphQLRequest {
    //   query: DocumentNode;
    //   variables?: Record<string, any>;
    //   operationName?: string;
    //   context?: Record<string, any>;
    //   extensions?: Record<string, any>;
    // }
    type t = {
      query: Graphql.documentNode,
      variables: option(Js.Json.t),
      operationName: option(string),
      context: option(Js.Json.t),
      extensions: option(Js.Json.t),
    };
  };

  type t = Js_.t;
};

module Operation = {
  module Js_ = {
    // export interface Operation {
    //     query: DocumentNode;
    //     variables: Record<string, any>;
    //     operationName: string;
    //     extensions: Record<string, any>;
    //     setContext: (context: Record<string, any>) => Record<string, any>;
    //     getContext: () => Record<string, any>;
    // }
    type t = {
      query: Graphql.documentNode,
      variables: Js.Json.t,
      operationName: string,
      extensions: Js.Json.t,
      setContext: Js.Json.t => Js.Json.t,
      getContext: unit => Js.Json.t,
    };
  };

  type t = Js_.t;
};

module FetchResult = {
  module Js_ = {
    // export interface FetchResult<TData = {
    //     [key: string]: any;
    // }, C = Record<string, any>, E = Record<string, any>> extends ExecutionResult {
    //     data?: TData | null;
    //     extensions?: E;
    //     context?: C;
    // }
    type t('jsData) = {
      data: Js.Nullable.t('jsData),
      extensions: option(Js.Json.t), // ACTUAL: Record<string, any>
      context: option(Js.Json.t), // ACTUAL: Record<string, any>
      // ...extends ExecutionResult
      errors: option(GraphQLError.t),
    };
  };

  type t('data) = {
    data: option('data),
    extensions: option(Js.Json.t), // ACTUAL: Record<string, any>
    context: option(Js.Json.t), // ACTUAL: Record<string, any>
    // ...extends ExecutionResult
    errors: option(GraphQLError.t),
  };

  let fromJs:
    (Js_.t('jsData), ~parse: ApolloClient__Types.parse('jsData, 'data)) =>
    t('data) =
    (js, ~parse) => {
      {
        data: js.data->Js.toOption->Belt.Option.map(parse),
        extensions: js.extensions,
        context: js.context,
        errors: js.errors,
      };
    };
};

module NextLink = {
  module Js_ = {
    // export declare type NextLink = (operation: Operation) => Observable<FetchResult>;
    type t = Operation.Js_.t => Observable.t(FetchResult.Js_.t(Js.Json.t));
  };
};

module RequestHandler = {
  module Js_ = {
    // export declare type RequestHandler = (operation: Operation, forward: NextLink) => Observable<FetchResult> | null;
    type t =
      (Operation.Js_.t, NextLink.Js_.t) =>
      Js.nullable(Observable.t(FetchResult.Js_.t(Js.Json.t)));
  };

  type t = Js_.t;
};
