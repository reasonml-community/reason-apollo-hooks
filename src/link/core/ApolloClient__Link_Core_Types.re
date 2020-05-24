module GraphQLError = ApolloClient__Graphql_Error_GraphQLError;

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
      data: option('jsData),
      extensions: option(Js.Json.t), // ACTUAL: Record<string, any>
      context: option(Js.Json.t), // ACTUAL: Record<string, any>
      // ...extends ExecutionResult
      errors: option(GraphQLError.t),
    };
  };

  type t('data) = Js_.t('data);

  let fromJs:
    (Js_.t('jsData), ~parse: ApolloClient__Types.parse('jsData, 'data)) =>
    t('data) =
    (js, ~parse) => {
      {
        data: js.data->Belt.Option.map(parse),
        extensions: js.extensions,
        context: js.context,
        errors: js.errors,
      };
    };
};
