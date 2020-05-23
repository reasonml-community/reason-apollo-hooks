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

type executionResult('raw_t) = {
  errors: Js.Nullable.t(Js.Array.t(graphqlError)),
  data: Js.Nullable.t('raw_t),
};

module type Definition = {
  let query: string;

  module Raw: {
    type t;
    type t_variables;
  };
  type t;

  let parse: Raw.t => t;
  let serialize: t => Raw.t;
};

type apolloError = {
  message: string,
  graphQLErrors: Js.Nullable.t(array(graphqlError)),
  networkError: Js.Nullable.t(string),
};

/**
 * apollo-client/src/core/networkStatus
 */
type networkStatus =
  | Loading
  | SetVariables
  | FetchMore
  | Refetch
  | Poll
  | Ready
  | Error
  | Unknown;

let toNetworkStatus = (status: Js.Nullable.t(int)) => {
  switch (status->Js.Nullable.toOption) {
  | Some(1) => Loading
  | Some(2) => SetVariables
  | Some(3) => FetchMore
  | Some(4) => Refetch
  | Some(6) => Poll
  | Some(7) => Ready
  | Some(8) => Error
  | _ => Unknown
  };
};

/**
 * apollo-client/src/core/watchQueryOptions.ts
 */
type fetchPolicy =
  | CacheFirst
  | CacheAndNetwork
  | NetworkOnly
  | CacheOnly
  | NoCache
  | Standby;

let fetchPolicyToJs = fetchPolicy => {
  switch (fetchPolicy) {
  | CacheFirst => "cache-first"
  | CacheAndNetwork => "cache-and-network"
  | NetworkOnly => "network-only"
  | CacheOnly => "cache-only"
  | NoCache => "no-cache"
  | Standby => "standby"
  };
};

/**
 * apollo-client/src/core/watchQueryOptions.ts
 */
type errorPolicy =
  | None
  | Ignore
  | All;

let errorPolicyToJs = errorPolicy =>
  switch (errorPolicy) {
  | None => "none"
  | Ignore => "ignore"
  | All => "all"
  };

type parse('raw_t, 't) = 'raw_t => 't;
type serialize('t, 'raw_t) = 't => 'raw_t;
type query = string;

type graphqlDefinition('t, 'raw_t) = (
  parse('raw_t, 't),
  query,
  serialize('t, 'raw_t),
);

module Context = {
  type t = Js.Dict.t(string);
  let make = (context): t => Js.Dict.fromList(context);
};

module type Operation = {
  let query: string;

  module Raw: {
    type t;
    type t_variables;
  };
  type t;

  let parse: Raw.t => t;
  let serialize: t => Raw.t;
};
