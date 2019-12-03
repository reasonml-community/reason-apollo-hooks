open ApolloHooksTypes;
type queryError = {. "message": string};

type variant('a) =
  | Data('a)
  | Error(queryError)
  | Loading
  | NoData;

/**
 *
 *  apollo-client/src/core/ObservableQuery.ts
 */
[@bs.deriving abstract]
type updateQueryOptions = {
  [@bs.optional]
  fetchMoreResult: Js.Json.t,
  [@bs.optional]
  variables: Js.Json.t,
};

type updateQueryT = (Js.Json.t, updateQueryOptions) => Js.Json.t;

type refetch('a) = (~variables: Js.Json.t=?, unit) => Js.Promise.t('a);
type queryResult('a) = {
  data: option('a),
  loading: bool,
  error: option(queryError),
  refetch: refetch('a),
  fetchMore:
    (~variables: Js.Json.t=?, ~updateQuery: updateQueryT, unit) =>
    Js.Promise.t(unit),
  networkStatus: ApolloHooksTypes.networkStatus,
};

/**
 * apollo-client/src/core/watchQueryOptions.ts
 */
[@bs.deriving abstract]
type fetchMoreOptions = {
  [@bs.optional]
  variables: Js.Json.t,
  updateQuery: updateQueryT,
};

[@bs.module "graphql-tag"] external gql: ReasonApolloTypes.gql = "default";

[@bs.deriving abstract]
type options = {
  [@bs.optional]
  variables: Js.Json.t,
  [@bs.optional]
  client: ApolloClient.generatedApolloClient,
  [@bs.optional]
  notifyOnNetworkStatusChange: bool,
  [@bs.optional]
  fetchPolicy: string,
  [@bs.optional]
  errorPolicy: string,
  [@bs.optional]
  skip: bool,
  [@bs.optional]
  pollInterval: int,
};

[@bs.module "@apollo/react-hooks"]
external useQueryJs:
  (ReasonApolloTypes.queryString, options) =>
  {
    .
    "data": Js.Nullable.t(Js.Json.t),
    "loading": bool,
    "error": Js.Nullable.t(queryError),
    [@bs.meth]
    "refetch": Js.Nullable.t(Js.Json.t) => Js.Promise.t(Js.Json.t),
    [@bs.meth] "fetchMore": fetchMoreOptions => Js.Promise.t(unit),
    "networkStatus": Js.Nullable.t(int),
  } =
  "useQuery";

let useQuery:
  (
    ~client: ApolloClient.generatedApolloClient=?,
    ~variables: Js.Json.t=?,
    ~notifyOnNetworkStatusChange: bool=?,
    ~fetchPolicy: ApolloHooksTypes.fetchPolicy=?,
    ~errorPolicy: ApolloHooksTypes.errorPolicy=?,
    ~skip: bool=?,
    ~pollInterval: int=?,
    graphqlDefinition('data, _, _)
  ) =>
  (variant('data), queryResult('data)) =
  (
    ~client=?,
    ~variables=?,
    ~notifyOnNetworkStatusChange=?,
    ~fetchPolicy=?,
    ~errorPolicy=?,
    ~skip=?,
    ~pollInterval=?,
    (parse, query, _),
  ) => {
    let jsResult =
      useQueryJs(
        gql(. query),
        options(
          ~variables?,
          ~client?,
          ~notifyOnNetworkStatusChange?,
          ~fetchPolicy=?
            fetchPolicy->Belt.Option.map(ApolloHooksTypes.fetchPolicyToJs),
          ~errorPolicy=?
            errorPolicy->Belt.Option.map(ApolloHooksTypes.errorPolicyToJs),
          ~skip?,
          ~pollInterval?,
          (),
        ),
      );

    let getData = obj =>
      obj
      ->Js.Json.decodeObject
      ->Belt.Option.flatMap(x => Js.Dict.get(x, "data"))
      ->Belt.Option.getExn;

    let result =
      React.useMemo1(
        () =>
          {
            data:
              jsResult##data
              ->Js.Nullable.toOption
              ->Belt.Option.flatMap(data =>
                  switch (parse(data)) {
                  | parsedData => Some(parsedData)
                  | exception _ => None
                  }
                ),
            loading: jsResult##loading,
            error: jsResult##error->Js.Nullable.toOption,
            networkStatus:
              ApolloHooksTypes.toNetworkStatus(jsResult##networkStatus),
            refetch: (~variables=?, ()) =>
              jsResult##refetch(Js.Nullable.fromOption(variables))
              |> Js.Promise.then_(result =>
                   parse(result->getData) |> Js.Promise.resolve
                 ),
            fetchMore: (~variables=?, ~updateQuery, ()) =>
              jsResult##fetchMore(
                fetchMoreOptions(~variables?, ~updateQuery, ()),
              ),
          },
        [|jsResult|],
      );

    let simple =
      React.useMemo1(
        () =>
          switch (result) {
          | {loading: true} => Loading
          | {error: Some(error)} => Error(error)
          | {data: Some(data)} => Data(data)
          | _ => NoData
          },
        [|result|],
      );

    (simple, result);
  };
