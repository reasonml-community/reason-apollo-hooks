module ApolloError = ApolloClient__ApolloError;
module ErrorPolicy = ApolloClient__Core_WatchQueryOptions.ErrorPolicy;
module Graphql = ApolloClient__Graphql;
module GraphqlTag = ApolloClient__GraphqlTag;
module QueryHookOptions = ApolloClient__React_Types.QueryHookOptions;
module QueryResult = ApolloClient__React_Types.QueryResult;
module WatchQueryFetchPolicy = ApolloClient__Core_WatchQueryOptions.WatchQueryFetchPolicy;

module type Operation = ApolloClient__Types.Operation;

type simpleQueryResult('a) =
  | Data('a)
  | Error(ApolloError.t)
  | Loading
  | NoData;

module Js_ = {
  // export declare function useQuery<TData = any, TVariables = OperationVariables>(query: DocumentNode, options?: QueryHookOptions<TData, TVariables>): QueryResult<TData, TVariables>;
  [@bs.module "@apollo/client"]
  external useQuery:
    (
      ~query: Graphql.documentNode,
      ~options: QueryHookOptions.Js_.t('jsData, 'variables)=?
    ) =>
    QueryResult.Js_.t('jsData, 'variables) =
    "useQuery";
};

let useQuery:
  type data variables jsData jsVariables.
    (
      ~client: ApolloClient__ApolloClient.t=?,
      ~context: Js.Json.t=?,
      ~displayName: string=?,
      ~errorPolicy: ErrorPolicy.t=?,
      ~fetchPolicy: WatchQueryFetchPolicy.t=?,
      ~notifyOnNetworkStatusChange: bool=?,
      ~onCompleted: data => unit=?,
      ~onError: ApolloError.t => unit=?,
      ~partialRefetch: bool=?,
      ~pollInterval: int=?,
      ~skip: bool=?,
      ~ssr: bool=?,
      ~variables: variables=?,
      (module Operation with
         type t = data and
         type Raw.t = jsData and
         type Raw.t_variables = jsVariables)
    ) =>
    QueryResult.t(data, variables) =
  (
    ~client=?,
    ~context=?,
    ~displayName=?,
    ~errorPolicy=?,
    ~fetchPolicy=?,
    ~notifyOnNetworkStatusChange=?,
    ~onCompleted=?,
    ~onError=?,
    ~partialRefetch=?,
    ~pollInterval=?,
    ~skip=?,
    ~ssr=?,
    ~variables=?,
    (module Definition),
  ) => {
    let jsQueryResult =
      Js_.useQuery(
        ~query=GraphqlTag.gql(Definition.query),
        ~options=
          QueryHookOptions.toJs(
            {
              client,
              context,
              displayName,
              errorPolicy,
              fetchPolicy,
              onCompleted,
              onError,
              notifyOnNetworkStatusChange,
              partialRefetch,
              pollInterval,
              query: None,
              skip,
              ssr,
              variables,
            },
            ~parse=Definition.parse,
          ),
      );

    ApolloClient__Utils.useGuaranteedMemo1(
      () => {
        jsQueryResult->QueryResult.fromJs(
          ~parse=Definition.parse,
          ~serialize=Definition.serialize,
        )
      },
      jsQueryResult,
    );
  };

module Extend = (M: Operation) => {
  let use =
      (
        ~client=?,
        ~context=?,
        ~displayName=?,
        ~errorPolicy=?,
        ~fetchPolicy=?,
        ~notifyOnNetworkStatusChange=?,
        ~onCompleted=?,
        ~onError=?,
        ~partialRefetch=?,
        ~pollInterval=?,
        ~skip=?,
        ~ssr=?,
        ~variables=?,
        (),
      ) => {
    useQuery(
      ~client?,
      ~context?,
      ~displayName?,
      ~errorPolicy?,
      ~fetchPolicy?,
      ~notifyOnNetworkStatusChange?,
      ~onCompleted?,
      ~onError?,
      ~partialRefetch?,
      ~pollInterval?,
      ~skip?,
      ~ssr?,
      ~variables?,
      (module M),
    );
  };
};
