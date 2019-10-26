type graphqlErrors;

type error = {
  .
  "message": string,
  "graphqlErrors": graphqlErrors,
};

type refetchQueries =
  ReasonApolloTypes.executionResult => array(ApolloClient.queryObj);

/* Result that is return by the hook */
type result('a) =
  | Data('a)
  | Error(error)
  | NoData;

/* Result that is return by the hook */
type controlledResult('a) = {
  loading: bool,
  called: bool,
  data: option('a),
  error: option(error),
};

type controlledVariantResult('a) =
  | Loading
  | Called
  | Data('a)
  | Error(error)
  | NoData;

[@bs.module "graphql-tag"] external gql: ReasonApolloTypes.gql = "default";

type mutationResult('a) = {. "data": 'a};

[@bs.deriving abstract]
type options('a) = {
  [@bs.optional]
  variables: Js.Json.t,
  [@bs.optional]
  mutation: option(ReasonApolloTypes.queryString),
  [@bs.optional]
  client: ApolloClient.generatedApolloClient,
  [@bs.optional]
  refetchQueries,
  [@bs.optional]
  awaitRefetchQueries: bool,
  [@bs.optional]
  update: (ApolloClient.generatedApolloClient, mutationResult('a)) => unit,
};

type jsResult = {
  .
  "data": Js.Nullable.t(Js.Json.t),
  "loading": bool,
  "called": bool,
  "error": Js.Nullable.t(error),
};

type jsMutate('a) = (. options('a)) => Js.Promise.t(jsResult);
type mutation('a, 'b) =
  (
    ~mutation: 'b,
    ~client: ApolloClient.generatedApolloClient=?,
    ~refetchQueries: refetchQueries=?,
    ~awaitRefetchQueries: bool=?,
    unit
  ) =>
  Js.Promise.t(controlledVariantResult('a));

[@bs.module "@apollo/react-hooks"]
external useMutationJs:
  (. ReasonApolloTypes.queryString, options('a)) => (jsMutate('a), jsResult) =
  "useMutation";

exception Error(string);

let useMutation =
    (
      ~mutation=?,
      ~client=?,
      ~refetchQueries=?,
      ~awaitRefetchQueries=?,
      ~update=?,
      (),
    ) => {
  let (jsMutate, jsResult) =
    useMutationJs(.
      switch (mutation) {
      | None => gql(. "mutation { emptyMutation }")
      | Some(mutation) => gql(. mutation##query)
      },
      options(
        ~variables=?
          switch (mutation) {
          | Some(mutation) => Some(mutation##variables)
          | None => None
          },
        ~client?,
        ~refetchQueries?,
        ~awaitRefetchQueries?,
        ~update?,
        (),
      ),
    );

  let parse = React.useRef(None);
  let mutate =
    React.useMemo1(
      (
        (),
        ~mutation as passedMutation: 'b=?,
        ~client=?,
        ~refetchQueries=?,
        ~awaitRefetchQueries=?,
        (),
      ) => {
        let mutation =
          switch (passedMutation, mutation) {
          | (Some(mutation), _) => mutation
          | (_, Some(mutation)) => mutation
          | (None, None) => raise(Error("Need to pass a mutation"))
          };

        React.Ref.setCurrent(parse, Some(mutation##parse));

        jsMutate(.
          options(
            ~variables=mutation##variables,
            ~mutation=Some(gql(. mutation##query)),
            ~client?,
            ~refetchQueries?,
            ~awaitRefetchQueries?,
            (),
          ),
        )
        |> Js.Promise.then_(jsResult =>
             (
               switch (
                 Js.Nullable.toOption(jsResult##data),
                 Js.Nullable.toOption(jsResult##error),
               ) {
               | (Some(data), _) =>
                 let parse = data => {
                   switch (React.Ref.current(parse)) {
                   | Some(parse) => parse(data)
                   | _ => raise(Error("Parse error"))
                   };
                 };
                 Data(parse(data));
               | (None, Some(error)) => Error(error)
               | (None, None) => NoData
               }
             )
             |> Js.Promise.resolve
           );
      },
      [|
        switch (mutation) {
        | Some(mutation) => Some(mutation##variables)
        | None => None
        },
      |],
    );

  let full =
    React.useMemo1(
      () => {
        let parse = data => {
          switch (React.Ref.current(parse)) {
          | Some(parse) => parse(data)
          | _ => raise(Error("Parse error"))
          };
        };
        {
          loading: jsResult##loading,
          called: jsResult##called,
          data: jsResult##data->Js.Nullable.toOption->Belt.Option.map(parse),
          error: jsResult##error->Js.Nullable.toOption,
        };
      },
      [|jsResult|],
    );

  let simple =
    React.useMemo1(
      () =>
        switch (full) {
        | {loading: true} => Loading
        | {error: Some(error)} => Error(error)
        | {data: Some(data)} => Data(data)
        | {called: true} => Called
        | _ => NoData
        },
      [|full|],
    );

  (mutate, simple, full);
};