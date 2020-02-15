/**
 * An abstract type to describe a query string object.
 */
type queryString;

/**
 * The signature of the `graphql-tag/gql` function that transforms a GraphQL
 * query string to the standard GraphQL AST.
 * https://github.com/apollographql/graphql-tag
 */
type gql = (. string) => queryString;

/**
 * An abstract type to describe an Apollo Link object.
 */
type apolloLink;

/**
 * An abstract type to describe an Apollo Cache object.
 */
type apolloCache;

type apolloErrorExtensions = {code: Js.Nullable.t(string)};

type graphqlError = {
  message: string,
  name: Js.Nullable.t(string),
  extensions: Js.Nullable.t(apolloErrorExtensions),
  locations: Js.Nullable.t(array(string)),
  path: Js.Nullable.t(array(string)),
  nodes: Js.Nullable.t(array(string)),
};

type executionResult = {
  errors: Js.Nullable.t(Js.Array.t(graphqlError)),
  data: Js.Nullable.t(Js.Json.t),
};

module type Config = {
  let query: string;
  type t;
  let parse: Js.Json.t => t;
};

type apolloError = {
  message: string,
  graphQLErrors: Js.Nullable.t(array(graphqlError)),
  networkError: Js.Nullable.t(string),
};
