open ApolloHooksTypes;
type error = {. "message": string};

type variant('a) =
  | Data('a)
  | Error(error)
  | Loading
  | NoData;

type result('a) = {
  data: option('a),
  loading: bool,
  error: option(error),
};

[@bs.module "graphql-tag"] external gql: ReasonApolloTypes.gql = "default";

[@bs.deriving abstract]
type options('raw_t) = {
  [@bs.optional]
  variables: Js.Json.t,
  [@bs.optional]
  skip: bool,
  [@bs.optional]
  onSubscriptionData: unit => unit,
  [@bs.optional]
  client: ApolloClient.generatedApolloClient('raw_t),
};

[@bs.module "@apollo/client"]
external useSubscription:
  (ReasonApolloTypes.queryString, options('raw_t)) =>
  {
    .
    "data": Js.Nullable.t(Js.Json.t),
    "loading": bool,
    "error": Js.Nullable.t(error),
  } =
  "useSubscription";

let useSubscription:
  (
    ~variables: Js.Json.t=?,
    ~client: ApolloClient.generatedApolloClient('raw_t)=?,
    ~skip: bool=?,
    ApolloHooksTypes.graphqlDefinition('t, 'raw_t, _)
  ) =>
  (variant('t), result('t)) =
  (~variables=?, ~client=?, ~skip=?, (parse, query, _)) => {
    let jsResult =
      useSubscription(
        gql(. query),
        options(~variables?, ~client?, ~skip?, ()),
      );

    let result = {
      data: jsResult##data->Js.Nullable.toOption->Belt.Option.map(parse),
      loading: jsResult##loading,
      error: jsResult##error->Js.Nullable.toOption,
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
