module type Config = {
  let query: string;
  type t;
  let parse: Js.Json.t => t;
};

module Make = (Config: Config) => {
  type error = {. "message": string};

  [@bs.module "react-apollo-hooks"]
  external useMutation:
    (
      string,
      {. "variables": Js.Nullable.t(Js.Json.t)},
      {. "variables": Js.Nullable.t(Js.Json.t)}
    ) =>
    Js.Promise.t({
      .
      "data": Js.Nullable.t(Js.Json.t),
      "error": Js.Nullable.t(error),
    }) =
    "useMutation";

  type result =
    | Data(Config.t)
    | Error(error)
    | NoData;

  let use = (~variables=?, ()) => {
    let jsMutate =
      useMutation(
        Config.query,
        {"variables": Js.Nullable.fromOption(variables)},
      );

    (~variables=?, ()) =>
      jsMutate({"variables": Js.Nullable.fromOption(variables)})
      |> Js.Promise.then_(jsResult =>
           (
             switch (
               Js.Nullable.toOption(jsResult##data),
               Js.Nullable.toOption(jsResult##error),
             ) {
             | (Some(data), _) => Data(Config.parse(data))
             | (None, Some(error)) => Error(error)
             | (None, None) => NoData
             }
           )
           |> Js.Promise.resolve
         );
  };
};
