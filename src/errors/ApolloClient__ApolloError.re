module Graphql = ApolloClient__Graphql;
module GraphQLError = Graphql.Error.GraphQLError;

// export declare class ApolloError extends Error {
//     constructor({ graphQLErrors, networkError, errorMessage, extraInfo, }: {
//         graphQLErrors?: ReadonlyArray<GraphQLError>;
//         networkError?: Error | null;
//         errorMessage?: string;
//         extraInfo?: any;
//     });
// }
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
};

type t = Js_.t;
