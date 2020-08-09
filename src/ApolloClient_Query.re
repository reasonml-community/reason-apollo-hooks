module Types = ApolloClient_Types;

type variant('a) =
  | Data('a)
  | Error(Types.apolloError)
  | Loading
  | NoData;

/**
 * https://github.com/apollographql/apollo-client/blob/master/packages/apollo-client/src/core/watchQueryOptions.ts#L139
 */
type updateSubscriptionOptionsJs('raw_t, 'raw_t_variables) = {
  .
  "subscriptionData": {. "data": 'raw_t},
  "variables": Js.Nullable.t('raw_t_variables),
};

type updateQuerySubscribeToMoreT('raw_t, 'raw_t_variables) =
  ('raw_t, updateSubscriptionOptionsJs('raw_t, 'raw_t_variables)) => 'raw_t;

[@bs.deriving abstract]
type subscribeToMoreOptionsJs('raw_t, 'raw_t_variables) = {
  document: Types.queryString,
  [@bs.optional]
  variables: 'raw_t_variables,
  [@bs.optional]
  updateQuery: updateQuerySubscribeToMoreT('raw_t, 'raw_t_variables),
};

type unsubscribeFnT = unit => unit;

type updateQueryOptions('t, 'raw_t_variables) = {
  fetchMoreResult: option('t),
  variables: option('raw_t_variables),
};

module Raw = {
  /**
   *
   *  apollo-client/src/core/ObservableQuery.ts
   */
  type updateQueryOptions('raw_t, 'raw_t_variables) = {
    fetchMoreResult: Js.Nullable.t('raw_t),
    variables: Js.Nullable.t('raw_t_variables),
  };

  /**
   * apollo-client/src/core/watchQueryOptions.ts
   */
  type fetchMoreOptions('raw_t, 'raw_t_variables) = {
    variables: option('raw_t_variables),
    updateQuery:
      option(
        ('raw_t, updateQueryOptions('raw_t, 'raw_t_variables)) => 'raw_t,
      ),
  };
};

type queryResult('t, 'raw_t, 't_variables, 'raw_t_variables) = {
  data: option('t),
  loading: bool,
  error: option(Types.apolloError),
  networkStatus: Types.networkStatus,
  refetch: (~variables: 't_variables=?, unit) => Js.Promise.t('t),
  fetchMore:
    (
      ~variables: 't_variables=?,
      ~updateQuery: ('t, updateQueryOptions('t, 'raw_t_variables)) => 't=?,
      unit
    ) =>
    Js.Promise.t(unit),
  rawFetchMore:
    (
      ~variables: 'raw_t_variables=?,
      ~updateQuery: (
                      'raw_t,
                      Raw.updateQueryOptions('raw_t, 'raw_t_variables)
                    ) =>
                    'raw_t
                      =?,
      unit
    ) =>
    Js.Promise.t(unit),
  startPolling: int => unit,
  stopPolling: unit => unit,
  subscribeToMore:
    (
      ~document: Types.queryString,
      ~variables: 't_variables=?,
      ~updateQuery: updateQuerySubscribeToMoreT('raw_t, 'raw_t_variables)=?,
      unit
    ) =>
    unsubscribeFnT,
};

[@bs.module "graphql-tag"] external gql: Types.gql = "default";

[@bs.deriving abstract]
type options('raw_t_variables) = {
  [@bs.optional]
  variables: 'raw_t_variables,
  [@bs.optional]
  client: ApolloClient_Client.t,
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
  [@bs.optional]
  context: Types.Context.t,
  [@bs.optional]
  ssr: bool,
  [@bs.optional]
  partialRefetch: bool,
};

type refetchResult('raw_t) = {data: Js.Nullable.t('raw_t)};

[@bs.module "@apollo/client"]
external useQueryJs:
  (Types.queryString, options('raw_t_variables)) =>
  {
    .
    "data": Js.Nullable.t('raw_t),
    "loading": bool,
    "error": Js.Nullable.t(Types.apolloError),
    [@bs.meth]
    "refetch":
      Js.Nullable.t('raw_t_variables) => Js.Promise.t(refetchResult('raw_t)),
    [@bs.meth]
    "fetchMore":
      Raw.fetchMoreOptions('raw_t, 'raw_t_variables) => Js.Promise.t(unit),
    "networkStatus": Js.Nullable.t(int),
    [@bs.meth] "stopPolling": unit => unit,
    [@bs.meth] "startPolling": int => unit,
    [@bs.meth]
    "subscribeToMore":
      subscribeToMoreOptionsJs('raw_t, 'raw_t_variables) => unsubscribeFnT,
  } =
  "useQuery";

let useQuery:
  type t t_variables raw_t raw_t_variables.
    (
      ~client: ApolloClient_Client.t=?,
      ~variables: t_variables,
      ~notifyOnNetworkStatusChange: bool=?,
      ~fetchPolicy: Types.fetchPolicy=?,
      ~errorPolicy: Types.errorPolicy=?,
      ~skip: bool=?,
      ~pollInterval: int=?,
      ~context: Types.Context.t=?,
      ~ssr: bool=?,
      ~partialRefetch: bool=?,
      (module Types.Operation with
         type t = t and
         type Raw.t = raw_t and
         type Raw.t_variables = raw_t_variables and
         type t_variables = t_variables)
    ) =>
    queryResult(t, raw_t, t_variables, raw_t_variables) =
  (
    ~client=?,
    ~variables,
    ~notifyOnNetworkStatusChange=?,
    ~fetchPolicy=?,
    ~errorPolicy=?,
    ~skip=?,
    ~pollInterval=?,
    ~context=?,
    ~ssr=?,
    ~partialRefetch=true,
    (module Operation),
  ) => {
    let jsResult =
      useQueryJs(
        gql(. Operation.query),
        options(
          ~variables=Operation.serializeVariables(variables),
          ~client?,
          ~notifyOnNetworkStatusChange?,
          ~fetchPolicy=?fetchPolicy->Belt.Option.map(Types.fetchPolicyToJs),
          ~errorPolicy=?errorPolicy->Belt.Option.map(Types.errorPolicyToJs),
          ~skip?,
          ~pollInterval?,
          ~context?,
          ~ssr?,
          ~partialRefetch,
          (),
        ),
      );

    React.useMemo1(
      () =>
        {
          data:
            jsResult##data
            ->Js.Nullable.toOption
            ->Belt.Option.flatMap(data =>
                switch (Operation.parse(data)) {
                | parsedData => Some(parsedData)
                | exception _ => None
                }
              ),
          loading: jsResult##loading,
          error: jsResult##error->Js.Nullable.toOption,
          networkStatus:
            ApolloClient_Types.toNetworkStatus(jsResult##networkStatus),
          refetch: (~variables=?, ()) =>
            jsResult##refetch(Js.Nullable.fromOption(switch(variables) { 
              | Some(variables) =>
                Some(Operation.serializeVariables(variables))
              | None => None 
            }))
            |> Js.Promise.then_(result =>
                 Operation.parse(
                   result.data->Js.Nullable.toOption->Belt.Option.getExn,
                 )
                 |> Js.Promise.resolve
               ),
          rawFetchMore: (~variables=?, ~updateQuery=?, ()) =>
            jsResult##fetchMore({Raw.variables: variables, Raw.updateQuery}),
          fetchMore: (~variables=?, ~updateQuery=?, ()) => {
            jsResult##fetchMore({
              Raw.variables: switch(variables) { 
                | Some (variables) =>
                  Some(Operation.serializeVariables(variables))
                | None => None
              },
              Raw.updateQuery:
                switch (updateQuery) {
                | Some(updateQuery) =>
                  Some(
                    (previousResult, Raw.{fetchMoreResult, variables}) => {
                      let result =
                        updateQuery(
                          Operation.parse(previousResult),
                          ({
                            fetchMoreResult:
                              switch (Js.Nullable.toOption(fetchMoreResult)) {
                              | None => None
                              | Some(fetchMoreResult) =>
                                Some(Operation.parse(fetchMoreResult))
                              },
                            variables:
                              (Js.Nullable.toOption(variables):
                               option(Operation.Raw.t_variables)),
                          }: updateQueryOptions(Operation.t,
                                                Operation.Raw.t_variables)),
                        );
                      Operation.serialize(result);
                    },
                  )
                | None => None
                },
            });
          },
          stopPolling: () => jsResult##stopPolling(),
          startPolling: interval => jsResult##startPolling(interval),
          subscribeToMore: (~document, ~variables=?, ~updateQuery=?, ()) =>
            jsResult##subscribeToMore(
              subscribeToMoreOptionsJs(
                ~document,
                ~variables=?{switch(variables) {
                  | Some(variables) =>
                    Some(Operation.serializeVariables(variables))
                  | None => None
                  }}
                  ,
                ~updateQuery?,
                (),
              ),
            ),
        },
      [|jsResult|],
    );
  };

let useQuery0:
  type t raw_t raw_t_variables t_variables.
    (
      ~client: ApolloClient_Client.t=?,
      ~notifyOnNetworkStatusChange: bool=?,
      ~fetchPolicy: Types.fetchPolicy=?,
      ~errorPolicy: Types.errorPolicy=?,
      ~skip: bool=?,
      ~pollInterval: int=?,
      ~context: Types.Context.t=?,
      ~ssr: bool=?,
      ~partialRefetch: bool=?,
      (module Types.OperationNoRequiredVars with
         type t = t and
         type Raw.t = raw_t and
         type Raw.t_variables = raw_t_variables and
         type t_variables = t_variables)
    ) =>
    queryResult(t, raw_t, t_variables, raw_t_variables) =
  (
    ~client=?,
    ~notifyOnNetworkStatusChange=?,
    ~fetchPolicy=?,
    ~errorPolicy=?,
    ~skip=?,
    ~pollInterval=?,
    ~context=?,
    ~ssr=?,
    ~partialRefetch=?,
    (module Operation),
  ) => {
    useQuery(
      ~client?,
      ~variables=Operation.makeDefaultVariables(),
      ~notifyOnNetworkStatusChange?,
      ~fetchPolicy?,
      ~errorPolicy?,
      ~skip?,
      ~pollInterval?,
      ~context?,
      ~ssr?,
      ~partialRefetch?,
      (module Operation),
    );
  };

let useQueryLegacy:
  type t raw_t raw_t_variables t_variables.
    (
      ~client: ApolloClient_Client.t=?,
      ~variables: t_variables=?,
      ~notifyOnNetworkStatusChange: bool=?,
      ~fetchPolicy: Types.fetchPolicy=?,
      ~errorPolicy: Types.errorPolicy=?,
      ~skip: bool=?,
      ~pollInterval: int=?,
      ~context: Types.Context.t=?,
      ~partialRefetch: bool=?,
      (module Types.Operation with
         type t = t and
         type Raw.t = raw_t and
         type Raw.t_variables = raw_t_variables and
         type t_variables = t_variables)
    ) =>
    (variant(t), queryResult(t, raw_t, t_variables, raw_t_variables)) =
  (
    ~client=?,
    ~variables=?,
    ~notifyOnNetworkStatusChange=?,
    ~fetchPolicy=?,
    ~errorPolicy=?,
    ~skip=?,
    ~pollInterval=?,
    ~context=?,
    ~partialRefetch=?,
    (module Operation),
  ) => {
    let jsResult =
      useQueryJs(
        gql(. Operation.query),
        options(
          ~variables=?{switch(variables) {
            | Some(variables) => Some(Operation.serializeVariables(variables))
            | None => None
            }},
          ~client?,
          ~notifyOnNetworkStatusChange?,
          ~fetchPolicy=?fetchPolicy->Belt.Option.map(Types.fetchPolicyToJs),
          ~errorPolicy=?errorPolicy->Belt.Option.map(Types.errorPolicyToJs),
          ~skip?,
          ~pollInterval?,
          ~context?,
          ~partialRefetch?,
          (),
        ),
      );

    let result =
      React.useMemo1(
        () =>
          {
            data:
              jsResult##data
              ->Js.Nullable.toOption
              ->Belt.Option.flatMap(data =>
                  switch (Operation.parse(data)) {
                  | parsedData => Some(parsedData)
                  | exception _ => None
                  }
                ),
            loading: jsResult##loading,
            error: jsResult##error->Js.Nullable.toOption,
            networkStatus:
              ApolloClient_Types.toNetworkStatus(jsResult##networkStatus),
            refetch: (~variables=?, ()) =>
              jsResult##refetch(Js.Nullable.fromOption(switch(variables) {
                | Some(variables) =>
                Some(Operation.serializeVariables(variables))
                | None => None
              }))
              |> Js.Promise.then_(result =>
                   Operation.parse(
                     result.data->Js.Nullable.toOption->Belt.Option.getExn,
                   )
                   |> Js.Promise.resolve
                 ),
            rawFetchMore: (~variables=?, ~updateQuery=?, ()) =>
              jsResult##fetchMore({Raw.variables: variables, Raw.updateQuery}),
            fetchMore: (~variables=?, ~updateQuery=?, ()) => {
              jsResult##fetchMore({
                Raw.variables: switch(variables) {
                  | Some(variables) =>
                    Some(Operation.serializeVariables(variables))
                  | None => None
                },
                Raw.updateQuery:
                  switch (updateQuery) {
                  | Some(updateQuery) =>
                    Some(
                      (previousResult, {fetchMoreResult, variables}) => {
                        let result =
                          updateQuery(
                            Operation.parse(previousResult),
                            {
                              fetchMoreResult:
                                switch (Js.Nullable.toOption(fetchMoreResult)) {
                                | None => None
                                | Some(fetchMoreResult) =>
                                  Some(Operation.parse(fetchMoreResult))
                                },
                              variables: Js.Nullable.toOption(variables),
                            },
                          );
                        Operation.serialize(result);
                      },
                    )
                  | None => None
                  },
              });
            },
            stopPolling: () => jsResult##stopPolling(),
            startPolling: interval => jsResult##startPolling(interval),
            subscribeToMore: (~document, ~variables=?, ~updateQuery=?, ()) =>
              jsResult##subscribeToMore(
                subscribeToMoreOptionsJs(
                  ~document,
                  ~variables=?{switch(variables) {
                    | Some(variables) =>
                      Some(Operation.serializeVariables(variables))
                    | None => None
                    }},
                  ~updateQuery?,
                  (),
                ),
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

module Extend = (M: Types.Operation) => {
  /**
  Use hook for apollo client
  */
  let use =
      (
        ~client=?,
        ~notifyOnNetworkStatusChange=?,
        ~fetchPolicy=?,
        ~errorPolicy=?,
        ~skip=?,
        ~pollInterval=?,
        ~context=?,
        ~ssr=?,
        ~partialRefetch=?,
        variables,
      ) => {
    useQuery(
      ~client?,
      ~variables,
      ~notifyOnNetworkStatusChange?,
      ~fetchPolicy?,
      ~errorPolicy?,
      ~skip?,
      ~pollInterval?,
      ~context?,
      ~ssr?,
      ~partialRefetch?,
      (module M),
    );
  };
};

module ExtendNoRequiredVars = (M: Types.OperationNoRequiredVars) => {
  /**
  Use hook for apollo client
  */
  let use =
      (
        ~client=?,
        ~variables=?,
        ~notifyOnNetworkStatusChange=?,
        ~fetchPolicy=?,
        ~errorPolicy=?,
        ~skip=?,
        ~pollInterval=?,
        ~context=?,
        ~ssr=?,
        ~partialRefetch=?,
        (),
      ) => {
    useQuery(
      ~client?,
      ~variables=
        switch (variables) {
        | Some(variables) => variables
        | None => M.makeDefaultVariables()
        },
      ~notifyOnNetworkStatusChange?,
      ~fetchPolicy?,
      ~errorPolicy?,
      ~skip?,
      ~pollInterval?,
      ~context?,
      ~ssr?,
      ~partialRefetch?,
      (module M),
    );
  };
};
