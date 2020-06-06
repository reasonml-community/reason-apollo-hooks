module Graphql = ApolloClient__Graphql;
module GraphqlTag = ApolloClient__GraphqlTag;
module FetchResult = ApolloClient__Link_Core_Types.FetchResult;
module Resolver = ApolloClient__Core_LocalState.Resolver;
module Types = ApolloClient__Reason_Types;

module PureQueryOptions = {
  module Js_ = {
    // export declare type PureQueryOptions = {
    //     query: DocumentNode;
    //     variables?: {
    //         [key: string]: any;
    //     };
    //     context?: any;
    // };
    type t('variables) = {
      query: Graphql.documentNode,
      variables: option('variables),
      context: option(Js.Json.t),
    };
  };

  type t('variables) = {
    query: string,
    variables: option('variables),
    context: option(Js.Json.t),
  };

  let toJs: t('variables) => Js_.t('variables) =
    t => {
      query: GraphqlTag.gql(t.query),
      variables: t.variables,
      context: t.context,
    };
};

module ApolloQueryResult = {
  module Js_ = {
    // export declare type ApolloQueryResult<T> = {
    //     data?: T;
    //     errors?: ReadonlyArray<GraphQLError>;
    //     loading: boolean;
    //     networkStatus: NetworkStatus;
    // };
    type t('jsData) = {
      data: option('jsData),
      errors: option(array(Graphql.Error.GraphQLError.t)),
      loading: bool,
      networkStatus: int,
    };
  };

  type t('parsedData) = Js_.t('parsedData);

  let fromJs: (t('jsData), ~parse: 'jsData => 'parsedData) => t('parsedData) =
    ({data, errors, loading, networkStatus}, ~parse) => {
      data: data->Belt.Option.map(parse),
      errors,
      loading,
      networkStatus,
    };
};

module MutationQueryReducer = {
  module Js_ = {
    // export declare type MutationQueryReducer<T> = (previousResult: Record<string, any>, options: {
    //     mutationResult: FetchResult<T>;
    //     queryName: string | undefined;
    //     queryVariables: Record<string, any>;
    // }) => Record<string, any>;
    type options('jsData) = {
      mutationResult: FetchResult.Js_.t('jsData),
      queryName: option(string),
      queryVariables: Js.Json.t // ACTUAL: Record<string, any>
    };

    type t('jsData) = (. Js.Json.t, options('jsData)) => Js.Json.t;
  };

  type options('data) = {
    mutationResult: FetchResult.t('data),
    queryName: option(string),
    queryVariables: Js.Json.t // ACTUAL: Record<string, any>
  };

  type t('data) = (Js.Json.t, options('data)) => Js.Json.t;

  let toJs:
    (t('data), ~parse: Types.parse('jsData, 'data)) =>
    (. Js.Json.t, Js_.options('jsData)) => Js.Json.t =
    (t, ~parse) =>
      (. previousResult, jsOptions) =>
        t(
          previousResult,
          {
            mutationResult:
              jsOptions.mutationResult->FetchResult.fromJs(~parse),
            queryName: jsOptions.queryName,
            queryVariables: jsOptions.queryVariables,
          },
        );
};

module MutationQueryReducersMap = {
  module Js_ = {
    // export declare type MutationQueryReducersMap<T = {
    //     [key: string]: any;
    // }> = {
    //     [queryName: string]: MutationQueryReducer<T>;
    // };
    type t('jsData) = Js.Dict.t(MutationQueryReducer.Js_.t('jsData));
  };

  type t('data) = Js.Dict.t(MutationQueryReducer.t('data));

  let toJs:
    (t('data), ~parse: Types.parse('jsData, 'data)) => Js_.t('jsData) =
    (t, ~parse) => {
      Js.Dict.map(
        (. mutationQueryReducer) =>
          mutationQueryReducer->MutationQueryReducer.toJs(~parse),
        t,
      );
    };
};

module Resolvers = {
  module Js_ = {
    // export interface Resolvers {
    //     [key: string]: {
    //         [field: string]: Resolver;
    //     };
    // }
    type t = Js.Dict.t(Js.Dict.t(Resolver.Js_.t));
  };
  type t = Js_.t;
};
