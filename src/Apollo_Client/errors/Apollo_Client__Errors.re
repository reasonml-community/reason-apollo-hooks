module Graphql = Apollo_Client__Graphql;

module ApolloError = {
  type t = {
    extraInfo: Js.Json.t,
    graphQLErrors: array(Graphql.Error.GraphQLError.t),
    networkError: Js.nullable(Js.Exn.t),
    // ...extends Error
    name: string,
    message: string,
    stack: option(string),
  };
  // type errorOptions ={ graphQLErrors, networkError, errorMessage, extraInfo};
  // [@bs.new] [@bs.module "@apollo/client"]
  // external make: errorOptions => t = "ApolloError";
};
