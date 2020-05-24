module ApolloError = ApolloClient__Errors.ApolloError;
module BaseSubscriptionOptions = ApolloClient__React_Types.BaseSubscriptionOptions;
module FetchPolicy = ApolloClient__Core_WatchQueryOptions.FetchPolicy;
module Graphql = ApolloClient__Graphql;
module GraphqlTag = ApolloClient__GraphqlTag;
module OnSubscriptionDataOptions = ApolloClient__React_Types.OnSubscriptionDataOptions;
module SubscriptionHookOptions = ApolloClient__React_Types.SubscriptionHookOptions;

module type Operation = ApolloClient__Types.Operation;

type simpleQueryResult('a) =
  | Data('a)
  | Error(ApolloError.t)
  | Loading
  | NoData;

module Js_ = {
  type useSubscription_result('jsData, 'variables) = {
    variables: option('variables),
    loading: bool,
    data: option('jsData),
    error: option(ApolloError.Js_.t),
  };

  // export declare function useSubscription<TData = any, TVariables = OperationVariables>(subscription: DocumentNode, options?: SubscriptionHookOptions<TData, TVariables>): {
  //     variables: TVariables | undefined;
  //     loading: boolean;
  //     data?: TData | undefined;
  //     error?: import("../..").ApolloError | undefined;
  // };
  [@bs.module "@apollo/client"]
  external useSubscription:
    (
      ~subscription: Graphql.Language.documentNode,
      ~options: SubscriptionHookOptions.Js_.t('jsData, 'variables)=?
    ) =>
    useSubscription_result('jsData, 'variables) =
    "useSubscription";
};

let useSubscription:
  type data variables jsData jsVariables.
    (
      ~client: ApolloClient__ApolloClient.t=?,
      ~fetchPolicy: FetchPolicy.t=?,
      ~onSubscriptionData: OnSubscriptionDataOptions.t(data) => unit=?,
      ~onSubscriptionComplete: unit => unit=?,
      ~shouldResubscribe: BaseSubscriptionOptions.t(data, variables) => bool=?,
      ~skip: bool=?,
      ~variables: variables=?,
      (module Operation with
         type t = data and
         type Raw.t = jsData and
         type Raw.t_variables = jsVariables)
    ) =>
    simpleQueryResult(data) =
  (
    ~client=?,
    ~fetchPolicy=?,
    ~onSubscriptionData=?,
    ~onSubscriptionComplete=?,
    ~shouldResubscribe=?,
    ~skip=?,
    ~variables=?,
    (module Definition),
  ) => {
    let jsSubscriptionResult =
      Js_.useSubscription(
        ~subscription=GraphqlTag.gql(Definition.query),
        ~options=
          SubscriptionHookOptions.toJs(
            {
              client,
              fetchPolicy,
              onSubscriptionData,
              onSubscriptionComplete,
              subscription: None,
              shouldResubscribe,
              skip,
              variables,
            },
            ~parse=Definition.parse,
          ),
      );

    ApolloClient__Utils.useGuaranteedMemo1(
      () => {
        switch (jsSubscriptionResult) {
        | {loading: true} => Loading
        | {error: Some(error)} => Error(error)
        | {data: Some(data)} => Data(data->Definition.parse)
        | _ => NoData
        }
      },
      jsSubscriptionResult,
    );
  };

module Extend = (M: Operation) => {
  let use =
      (
        ~client=?,
        ~fetchPolicy=?,
        ~onSubscriptionData=?,
        ~onSubscriptionComplete=?,
        ~shouldResubscribe=?,
        ~skip=?,
        ~variables=?,
        (),
      ) => {
    useSubscription(
      ~client?,
      ~fetchPolicy?,
      ~onSubscriptionData?,
      ~onSubscriptionComplete?,
      ~shouldResubscribe?,
      ~skip?,
      ~variables?,
      (module M),
    );
  };
};
