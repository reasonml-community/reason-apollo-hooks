open ReasonApolloTypes;

type queryObj('raw_t_variables) = {
  query: ReasonApolloTypes.queryString,
  variables: 'raw_t_variables,
};

type opaqueQueryObj;
external toOpaqueQueryObj: queryObj('raw_t_variables) => opaqueQueryObj =
  "%identity";

type mutationObj('raw_t_variables) = {
  mutation: ReasonApolloTypes.queryString,
  variables: 'raw_t_variables,
};

type updateQueryOptions('raw_t, 'raw_t_variables) = {
  fetchMoreResult: option('raw_t),
  variables: option('raw_t_variables),
};

type onErrorT;
type updateQueryT('raw_t, 'raw_t_variables) =
  ('raw_t, updateQueryOptions('raw_t, 'raw_t_variables)) => 'raw_t;

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

type fetchMoreOptions('raw_t, 'raw_t_variables) = {
  variables: option('raw_t_variables),
  updateQuery: updateQueryT('raw_t, 'raw_t_variables),
};

type queryResult('raw_t, 'raw_t_variables) = {
  loading: bool,
  data: Js.Nullable.t('raw_t),
  error: Js.Nullable.t(apolloError),
  refetch:
    Js.Null_undefined.t('raw_t) =>
    Js.Promise.t(queryResult('raw_t, 'raw_t_variables)),
  networkStatus: Js.Nullable.t(int),
  variables: Js.Null_undefined.t(Js.Json.t),
  fetchMore:
    fetchMoreOptions('raw_t, 'raw_t_variables) => Js.Promise.t(unit),
  subscribeToMore: subscribeToMoreOptions => unit,
};

type mutationResult('raw_t) = {
  loading: bool,
  called: bool,
  data: Js.Nullable.t('raw_t),
  error: Js.Nullable.t(apolloError),
  networkStatus: Js.Nullable.t(int),
  variables: Js.Null_undefined.t(Js.Json.t),
};

type t;

[@bs.send]
external query:
  (t, queryObj('raw_t_variables)) =>
  Js.Promise.t(queryResult('raw_t, 'raw_t_variables)) =
  "query";

[@bs.send]
external mutate:
  (t, mutationObj('raw_t_variables)) => Js.Promise.t(mutationResult('raw_t)) =
  "mutate";
[@bs.send] external resetStore: t => Js.Promise.t(unit) = "resetStore";

type apolloClientObjectParam = {
  link: apolloLink,
  cache: apolloCache,
  ssrMode: option(bool),
  ssrForceFetchDelay: option(int),
  connectToDevTools: option(bool),
  queryDeduplication: option(bool),
};

[@bs.module "@apollo/client"] [@bs.new]
external createApolloClientJS: apolloClientObjectParam => t = "ApolloClient";

[@bs.module "graphql-tag"] external gql: ReasonApolloTypes.gql = "default";

module ReadQuery = (Config: ReasonApolloTypes.Config) => {
  type readQueryOptions = {
    query: ReasonApolloTypes.queryString,
    variables: Js.Nullable.t(Config.Raw.t_variables),
  };
  type response = option(Config.t);
  [@bs.send]
  external readQuery: (t, readQueryOptions) => Js.Nullable.t(Config.Raw.t) =
    "readQuery";

  let graphqlQueryAST = gql(. Config.query);
  let apolloDataToRecord: Js.Nullable.t(Config.Raw.t) => response =
    apolloData =>
      Js.Nullable.toOption(apolloData)->(Belt.Option.map(Config.parse));

  let make = (~client, ~variables: option(Config.Raw.t_variables)=?, ()) =>
    readQuery(
      client,
      {query: graphqlQueryAST, variables: Js.Nullable.fromOption(variables)},
    )
    ->apolloDataToRecord;
};

module WriteQuery = (Config: ReasonApolloTypes.Config) => {
  type writeQueryOptions = {
    query: ReasonApolloTypes.queryString,
    variables: Js.Nullable.t(Config.Raw.t_variables),
    data: Config.t,
  };

  [@bs.send]
  external writeQuery: (t, writeQueryOptions) => unit = "writeQuery";

  let graphqlQueryAST = gql(. Config.query);

  let make =
      (
        ~client,
        ~variables: option(Config.Raw.t_variables)=?,
        ~data: Config.t,
        (),
      ) =>
    writeQuery(
      client,
      {
        query: graphqlQueryAST,
        variables: Js.Nullable.fromOption(variables),
        data,
      },
    );
};
