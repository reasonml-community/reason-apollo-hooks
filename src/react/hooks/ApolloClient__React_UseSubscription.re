module ApolloError = ApolloClient__ApolloError;
module BaseSubscriptionOptions = ApolloClient__React_Types.BaseSubscriptionOptions;
module FetchPolicy = ApolloClient__Core_WatchQueryOptions.FetchPolicy;
module Graphql = ApolloClient__Graphql;
module GraphqlTag = ApolloClient__GraphqlTag;
module OnSubscriptionDataOptions = ApolloClient__React_Types.OnSubscriptionDataOptions;
module SubscriptionHookOptions = ApolloClient__React_Types.SubscriptionHookOptions;
module Types = ApolloClient__Reason_Types;

module type Operation = ApolloClient__Reason_Types.Operation;
module type OperationNoRequiredVars = ApolloClient__Reason_Types.OperationNoRequiredVars;

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

type useSubscription_result('data, 'variables) = {
  variables: option('variables),
  loading: bool,
  data: option('data),
  error: option(ApolloError.Js_.t),
};

let useSubscription:
  type data jsData jsVariables.
    (
      ~client: ApolloClient__ApolloClient.t=?,
      ~fetchPolicy: FetchPolicy.t=?,
      ~onSubscriptionData: OnSubscriptionDataOptions.t(data) => unit=?,
      ~onSubscriptionComplete: unit => unit=?,
      ~shouldResubscribe: BaseSubscriptionOptions.t(data, jsVariables) => bool
                            =?,
      ~skip: bool=?,
      ~variables: jsVariables,
      (module Operation with
         type t = data and
         type Raw.t = jsData and
         type Raw.t_variables = jsVariables)
    ) =>
    useSubscription_result(data, jsVariables) =
  (
    ~client=?,
    ~fetchPolicy=?,
    ~onSubscriptionData=?,
    ~onSubscriptionComplete=?,
    ~shouldResubscribe=?,
    ~skip=?,
    ~variables,
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
              variables: Some(variables),
            },
            ~parse=Definition.parse,
          ),
      );

    ApolloClient__Reason_Utils.useGuaranteedMemo1(
      () =>
        {
          variables: jsSubscriptionResult.variables,
          loading: jsSubscriptionResult.loading,
          data: jsSubscriptionResult.data->Belt.Option.map(Definition.parse),
          error: jsSubscriptionResult.error,
        },
      jsSubscriptionResult,
    );
  };

let useSubscription0:
  type data jsData jsVariables.
    (
      ~client: ApolloClient__ApolloClient.t=?,
      ~fetchPolicy: FetchPolicy.t=?,
      ~onSubscriptionData: OnSubscriptionDataOptions.t(data) => unit=?,
      ~onSubscriptionComplete: unit => unit=?,
      ~shouldResubscribe: BaseSubscriptionOptions.t(data, jsVariables) => bool
                            =?,
      ~skip: bool=?,
      (module Types.OperationNoRequiredVars with
         type t = data and
         type Raw.t = jsData and
         type Raw.t_variables = jsVariables)
    ) =>
    useSubscription_result(data, jsVariables) =
  (
    ~client=?,
    ~fetchPolicy=?,
    ~onSubscriptionData=?,
    ~onSubscriptionComplete=?,
    ~shouldResubscribe=?,
    ~skip=?,
    (module Definition),
  ) => {
    useSubscription(
      ~client?,
      ~fetchPolicy?,
      ~onSubscriptionData?,
      ~onSubscriptionComplete?,
      ~shouldResubscribe?,
      ~skip?,
      ~variables=
        ApolloClient__Reason_Utils.nullAsDefaultVariables(
          Definition.makeDefaultVariables(),
        ),
      (module Definition),
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
        ~variables,
        (),
      ) => {
    useSubscription(
      ~client?,
      ~fetchPolicy?,
      ~onSubscriptionData?,
      ~onSubscriptionComplete?,
      ~shouldResubscribe?,
      ~skip?,
      ~variables,
      (module M),
    );
  };
};

module ExtendNoRequiredVariables = (M: OperationNoRequiredVars) => {
  let use =
      (
        ~client=?,
        ~fetchPolicy=?,
        ~onSubscriptionData=?,
        ~onSubscriptionComplete=?,
        ~shouldResubscribe=?,
        ~skip=?,
        (),
      ) => {
    useSubscription0(
      ~client?,
      ~fetchPolicy?,
      ~onSubscriptionData?,
      ~onSubscriptionComplete?,
      ~shouldResubscribe?,
      ~skip?,
      (module M),
    );
  };
};
