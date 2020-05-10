module Core = Apollo_Client__Core;
module Errors = Apollo_Client__Errors;
module Graphql = Apollo_Client__Graphql;
module GraphqlTag = Apollo_Client__GraphqlTag;
module React_Types = Apollo_Client__React_Types;

type simpleQueryResult('a) =
  | Data('a)
  | Error(Errors.ApolloError.t)
  | Loading
  | NoData;

module Raw = {
  [@bs.module "@apollo/client"]
  external useQuery:
    (
      ~query: Graphql.Language.documentNode,
      ~options: React_Types.QueryHookOptions.t('raw_tData, 'raw_tVariables)=?
    ) =>
    React_Types.QueryResult.Raw.t('raw_tData, 'raw_tVariables) =
    "useQuery";
};

let useQuery:
  (
    ~client: Apollo_Client__ApolloClient.t=?,
    ~context: Js.Json.t=?,
    ~displayName: string=?,
    ~errorPolicy: Core.WatchQueryOptions.ErrorPolicy.t=?,
    ~fetchPolicy: Core.WatchQueryOptions.WatchQueryFetchPolicy.t=?,
    ~notifyOnNetworkStatusChange: bool=?,
    ~onCompleted: 'raw_tData => unit=?,
    ~onError: Errors.ApolloError.t => unit=?,
    ~partialRefetch: bool=?,
    ~pollInterval: int=?,
    ~skip: bool=?,
    ~ssr: bool=?,
    ~variables: 'raw_tVariables=?,
    Apollo_Client__Types.graphqlDefinition('tData, 'raw_tData)
  ) =>
  (
    simpleQueryResult('tData),
    React_Types.QueryResult.t('tData, 'raw_tVariables),
  ) =
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
    (parse, query, serialize),
  ) => {
    let jsQueryResult =
      Raw.useQuery(
        ~query=GraphqlTag.gql(query),
        ~options={
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
      );

    Apollo_Client__Utils.useGuaranteedMemo1(
      () => {
        let queryResult =
          jsQueryResult->React_Types.QueryResult.fromRaw(~parse, ~serialize);

        let simple =
          Apollo_Client__Utils.useGuaranteedMemo1(
            () =>
              switch (queryResult) {
              | {loading: true} => Loading
              | {error: Some(error)} => Error(error)
              | {data: Some(data)} => Data(data)
              | _ => NoData
              },
            [|queryResult|],
          );

        (simple, queryResult);
      },
      jsQueryResult,
    );
  };
