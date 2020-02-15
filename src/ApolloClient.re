open ReasonApolloTypes;

type queryObj = {
  query: ReasonApolloTypes.queryString,
  variables: Js.Json.t,
};

type mutationObj = {
  mutation: ReasonApolloTypes.queryString,
  variables: Js.Json.t,
};

type updateQueryOptions = {
  fetchMoreResult: option(Js.Json.t),
  variables: option(Js.Json.t),
};

type onErrorT;
type updateQueryT = (Js.Json.t, updateQueryOptions) => Js.Json.t;
type updateSubscriptionOptions = {
  subscriptionData: option(Js.Json.t),
  variables: option(Js.Json.t),
};
type updateQuerySubscriptionT =
  (Js.Json.t, updateSubscriptionOptions) => Js.Json.t;

type subscribeToMoreOptions = {
  document: queryString,
  variables: option(Js.Json.t),
  updateQuery: option(updateQuerySubscriptionT),
  onError: option(onErrorT),
};

type fetchMoreOptions = {
  variables: option(Js.Json.t),
  updateQuery: updateQueryT,
};

type queryResult = {
  loading: bool,
  data: Js.Nullable.t(Js.Json.t),
  error: Js.Nullable.t(apolloError),
  refetch: Js.Null_undefined.t(Js.Json.t) => Js.Promise.t(queryResult),
  networkStatus: Js.Nullable.t(int),
  variables: Js.Null_undefined.t(Js.Json.t),
  fetchMore: fetchMoreOptions => Js.Promise.t(unit),
  subscribeToMore: subscribeToMoreOptions => unit,
};

type mutationResult = {
  loading: bool,
  called: bool,
  data: Js.Nullable.t(Js.Json.t),
  error: Js.Nullable.t(apolloError),
  networkStatus: Js.Nullable.t(int),
  variables: Js.Null_undefined.t(Js.Json.t),
};


type generatedApolloClient = {
  query:
    [@bs.meth] (queryObj => Js.Promise.t(queryResult)),
  mutate:
    [@bs.meth] (
      mutationObj => Js.Promise.t(mutationResult)
    ),
  resetStore: [@bs.meth] (unit => Js.Promise.t(unit)),
};

type apolloClientObjectParam = {
  link: apolloLink,
  cache: apolloCache,
  ssrMode: option(bool),
  ssrForceFetchDelay: option(int),
  connectToDevTools: option(bool),
  queryDeduplication: option(bool),
};

[@bs.module "@apollo/client"] [@bs.new]
external createApolloClientJS: apolloClientObjectParam => generatedApolloClient =
  "ApolloClient";

[@bs.module "graphql-tag"] external gql: ReasonApolloTypes.gql = "default";

module ReadQuery = (Config: ReasonApolloTypes.Config) => {
  type readQueryOptions = {
    query: ReasonApolloTypes.queryString,
    variables: Js.Nullable.t(Js.Json.t),
  };
  type response = option(Config.t);
  [@bs.send]
  external readQuery:
    (generatedApolloClient, readQueryOptions) => Js.Nullable.t(Js.Json.t) =
    "readQuery";

  let graphqlQueryAST = gql(. Config.query);
  let apolloDataToRecord: Js.Nullable.t(Js.Json.t) => response =
    apolloData =>
      Js.Nullable.toOption(apolloData)->(Belt.Option.map(Config.parse));

  let make = (~client, ~variables: option(Js.Json.t)=?, ()) =>
    readQuery(
      client,
      {query: graphqlQueryAST, variables: Js.Nullable.fromOption(variables)},
    )
    ->apolloDataToRecord;
};

module WriteQuery = (Config: ReasonApolloTypes.Config) => {
  type writeQueryOptions = {
    query: ReasonApolloTypes.queryString,
    variables: Js.Nullable.t(Js.Json.t),
    data: Config.t,
  };
  [@bs.send]
  external writeQuery: (generatedApolloClient, writeQueryOptions) => unit =
    "writeQuery";

  let graphqlQueryAST = gql(. Config.query);

  let make = (~client, ~variables: option(Js.Json.t)=?, ~data: Config.t, ()) =>
    writeQuery(
      client,
      {
        query: graphqlQueryAST,
        variables: Js.Nullable.fromOption(variables),
        data,
      },
    );
};
