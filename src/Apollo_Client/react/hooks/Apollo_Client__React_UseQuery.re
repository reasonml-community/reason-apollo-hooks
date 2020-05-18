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

module JS = {
  [@bs.module "@apollo/client"]
  external useQuery:
    (
      ~query: Graphql.Language.documentNode,
      ~options: React_Types.QueryHookOptions.t('jsData, 'variables)=?
    ) =>
    React_Types.QueryResult.JS.t('jsData, 'variables) =
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
    ~onCompleted: 'jsData => unit=?,
    ~onError: Errors.ApolloError.t => unit=?,
    ~partialRefetch: bool=?,
    ~pollInterval: int=?,
    ~skip: bool=?,
    ~ssr: bool=?,
    ~variables: 'variables=?,
    Apollo_Client__Types.graphqlDefinition('parsedData, 'jsData)
  ) =>
  (
    simpleQueryResult('parsedData),
    React_Types.QueryResult.t('parsedData, 'variables),
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
      JS.useQuery(
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
          jsQueryResult->React_Types.QueryResult.fromJs(~parse, ~serialize);

        let simple =
          switch (queryResult) {
          | {loading: true} => Loading
          | {error: Some(error)} => Error(error)
          | {data: Some(data)} => Data(data)
          | _ => NoData
          };

        (simple, queryResult);
      },
      jsQueryResult,
    );
  };
