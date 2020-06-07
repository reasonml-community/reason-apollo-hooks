module Graphql = ApolloClient__Graphql;
module GraphQLError = Graphql.Error.GraphQLError;

module Js_ = {
  // export declare class ApolloError extends Error {
  //     message: string;
  //     graphQLErrors: ReadonlyArray<GraphQLError>;
  //     networkError: Error | null;
  //     extraInfo: any;
  // }
  type t = {
    extraInfo: Js.Json.t,
    graphQLErrors: array(Graphql.Error.GraphQLError.t),
    networkError: Js.nullable(Js.Exn.t),
    // ...extends Error
    name: string,
    message: string,
    stack: option(string),
  };

  type make_args = {
    graphQLErrors: option(array(GraphQLError.t)),
    networkError: Js.nullable(Js.Exn.t),
    errorMessage: option(string),
    extraInfo: option(Js.Json.t),
  };

  //     constructor({ graphQLErrors, networkError, errorMessage, extraInfo, }: {
  //         graphQLErrors?: ReadonlyArray<GraphQLError>;
  //         networkError?: Error | null;
  //         errorMessage?: string;
  //         extraInfo?: any;
  //     });
  [@bs.module "@apollo/client"] [@bs.new]
  external make: make_args => t = "ApolloError";
};

type t = Js_.t;

let make:
  (
    ~graphQLErrors: array(GraphQLError.t)=?,
    ~networkError: Js.Exn.t=?,
    ~errorMessage: string=?,
    ~extraInfo: Js.Json.t=?,
    unit
  ) =>
  t =
  (~graphQLErrors=?, ~networkError=?, ~errorMessage=?, ~extraInfo=?, ()) =>
    Js_.make({
      graphQLErrors,
      networkError: Js.Nullable.fromOption(networkError),
      errorMessage,
      extraInfo,
    });
