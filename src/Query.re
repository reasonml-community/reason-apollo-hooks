module type Config = {
  let query: string;
  type t;
  let parse: Js.Json.t => t;
};

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
  refetch: (~variables: option(Js.Json.t)=?, unit) => Js.Promise.t('a),
};

module Make = (Config: Config) => {
  [@bs.module] external gql: ReasonApolloTypes.gql = "graphql-tag";

  [@bs.deriving abstract]
  type options = {
    [@bs.optional]
    variables: Js.Json.t,
    [@bs.optional]
    client: ApolloClient.generatedApolloClient,
  };

  [@bs.module "@apollo/react-hooks"]
  external useQuery:
    (ReasonApolloTypes.queryString, options) =>
    {
      .
      "data": Js.Nullable.t(Js.Json.t),
      "loading": bool,
      "error": Js.Nullable.t(error),
      [@bs.meth]
      "refetch":
        Js.Nullable.t(option(Js.Json.t)) => Js.Promise.t(Js.Json.t),
    } =
    "useQuery";

  let use = (~variables=?, ~client=?, ()) => {
    let jsResult =
      useQuery(gql(. Config.query), options(~variables?, ~client?, ()));

    let result = {
      data:
        jsResult##data
        ->Js.Nullable.toOption
        ->Belt.Option.flatMap(data =>
            switch (Config.parse(data)) {
            | parsedData => Some(parsedData)
            | exception _ => None
            }
          ),
      loading: jsResult##loading,
      error: jsResult##error->Js.Nullable.toOption,
      refetch: (~variables=?, ()) =>
        jsResult##refetch(Js.Nullable.fromOption(variables))
        |> Js.Promise.then_(result =>
             Config.parse(result) |> Js.Promise.resolve
           ),
    };

    (
      switch (result) {
      | {loading: true} => Loading
      | {error: Some(error)} => Error(error)
      | {data: Some(data)} => Data(data)
      | _ => NoData
      },
      result,
    );
  };
};
