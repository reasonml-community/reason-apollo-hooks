module Types = ApolloClient_Types;

type variant('a) =
  | Data('a)
  | Error(Types.apolloError)
  | Loading
  | NoData;

type result('a) = {
  data: option('a),
  loading: bool,
  error: option(Types.apolloError),
};

module Raw = {
  type result('raw_t) = {
    data: Js.Nullable.t('raw_t),
    loading: bool,
    error: Js.Nullable.t(Types.apolloError),
  };
};

[@bs.module "graphql-tag"] external gql: Types.gql = "default";

[@bs.deriving abstract]
type options('raw_t, 'raw_t_variables) = {
  [@bs.optional]
  variables: 'raw_t_variables,
  [@bs.optional]
  skip: bool,
  [@bs.optional]
  onSubscriptionData: unit => unit,
  [@bs.optional]
  client: ApolloClient_Client.t,
};

[@bs.module "@apollo/client"]
external useSubscription:
  (Types.queryString, options('raw_t, 'raw_t_variables)) =>
  Raw.result('raw_t) =
  "useSubscription";

let useSubscription:
  type t t_variables raw_t raw_t_variables.
    (
      ~variables: t_variables=?,
      ~client: ApolloClient_Client.t=?,
      ~skip: bool=?,
      (module Types.Operation with
         type t = t and
         type Raw.t = raw_t and
         type t_variables = t_variables and
         type Raw.t_variables = raw_t_variables)
    ) =>
    (variant(t), result(t)) =
  (~variables=?, ~client=?, ~skip=?, (module Operation)) => {
    let jsResult =
      useSubscription(
        gql(. Operation.query),
        options(~variables=?{switch(variables) {
          | Some(variables) => Some(Operation.serializeVariables(variables))
          | None => None }}, ~client?, ~skip?, ()),
      );

    let result = {
      data:
        jsResult.data->Js.Nullable.toOption->Belt.Option.map(Operation.parse),
      loading: jsResult.loading,
      error: jsResult.error->Js.Nullable.toOption,
    };

    (
      switch (result) {
      | {data: Some(data)} => Data(data)
      | {error: Some(error)} => Error(error)
      | {loading: true} => Loading
      | _ => NoData
      },
      result,
    );
  };
