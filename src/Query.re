module type Config = {
  let query: string;
  type t;
  let parse: Js.Json.t => t;
};

module Make = (Config: Config) => {
  type error = {. "message": string};

  [@bs.module "react-apollo-hooks"]
  external useQuery:
    (string, {. "variables": Js.Nullable.t(Js.Json.t)}) =>
    {
      .
      "data": Js.Nullable.t(Js.Json.t),
      "loading": bool,
      "error": Js.Nullable.t(error),
    } =
    "useQuery";

  type variant =
    | Data(Config.t)
    | Error(error)
    | Loading
    | NoData;
  type result = {
    data: option(Config.t),
    loading: bool,
    error: option(error),
  };

  let use = (~variables=?, ()) => {
    let jsResult =
      useQuery(
        Config.query,
        {"variables": Js.Nullable.fromOption(variables)},
      );

    let result = {
      data:
        jsResult##data->Js.Nullable.toOption->Belt.Option.map(Config.parse),
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
};
