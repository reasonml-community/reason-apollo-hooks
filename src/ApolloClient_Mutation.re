module Types = ApolloClient_Types;

type jsResult('raw_t) = {
  .
  "data": Js.Nullable.t('raw_t),
  "loading": bool,
  "called": bool,
  "error": Js.Nullable.t(Types.apolloError),
};

module Execution = {
  type jsExecutionResult('raw_t) = {
    .
    "data": Js.Nullable.t('raw_t),
    "errors": Js.Nullable.t(array(Types.graphqlError)),
  };

  type refetchQueries('raw_t) =
    jsExecutionResult('raw_t) => array(ApolloClient_Client.opaqueQueryObj);

  /* The type that the promise returned by the mutate function resolves to */
  type executionResult('a) = {
    data: option('a),
    errors: option(array(Types.graphqlError)),
  };

  type executionVariantResult('a) =
    | Data('a)
    | Errors(array(Types.graphqlError))
    | NoData;
};

/* The type of the 'full' result returned by the hook */
type controlledResult('t) = {
  loading: bool,
  called: bool,
  data: option('t),
  error: option(Types.apolloError),
};

/* The type of the 'simple' result returned by the hook */
type controlledVariantResult('t) =
  | Loading
  | NotCalled
  | Data('t)
  | Error(Types.apolloError)
  | NoData;

[@bs.module "graphql-tag"] external gql: Types.gql = "default";

type mutationResult('raw_t) = {. "data": option('raw_t)};

[@bs.deriving abstract]
type options('raw_t, 'raw_t_variables) = {
  [@bs.optional]
  variables: 'raw_t_variables,
  [@bs.optional]
  mutation: option(Types.queryString),
  [@bs.optional]
  client: ApolloClient_Client.t,
  [@bs.optional]
  refetchQueries: Execution.refetchQueries('raw_t),
  [@bs.optional]
  awaitRefetchQueries: bool,
  [@bs.optional]
  update: (ApolloClient_Client.t, mutationResult('raw_t)) => unit,
  [@bs.optional]
  optimisticResponse: 'raw_t,
};

type jsMutate('raw_t, 'raw_t_variables) =
  (. options('raw_t, 'raw_t_variables)) =>
  Js.Promise.t(Execution.jsExecutionResult('raw_t));

type mutation0('t, 'raw_t, 'raw_t_variables) =
  (
    ~client: ApolloClient_Client.t=?,
    ~refetchQueries: Execution.refetchQueries('raw_t)=?,
    ~awaitRefetchQueries: bool=?,
    ~optimisticResponse: 't=?,
    ~variables: 'raw_t_variables=?,
    unit
  ) =>
  Js.Promise.t(
    (Execution.executionVariantResult('t), Execution.executionResult('t)),
  );

type mutation('t, 'raw_t, 'raw_t_variables) =
  (
    ~client: ApolloClient_Client.t=?,
    ~refetchQueries: Execution.refetchQueries('raw_t)=?,
    ~awaitRefetchQueries: bool=?,
    ~optimisticResponse: 't=?,
    'raw_t_variables
  ) =>
  Js.Promise.t(
    (Execution.executionVariantResult('t), Execution.executionResult('t)),
  );

type mutationNoVars('t, 'raw_t, 'raw_t_variables) =
  unit =>
  Js.Promise.t(
    (Execution.executionVariantResult('t), Execution.executionResult('t)),
  );

[@bs.module "@apollo/client"]
external useMutationJs:
  (. Types.queryString, options('raw_t, 'raw_t_variables)) =>
  (jsMutate('raw_t, 'raw_t_variables), jsResult('raw_t)) =
  "useMutation";

exception Error(string);

let useMutation:
  type t raw_t raw_t_variables.
    (
      ~client: ApolloClient_Client.t=?,
      ~refetchQueries: Execution.refetchQueries(raw_t)=?,
      ~awaitRefetchQueries: bool=?,
      ~update: (ApolloClient_Client.t, mutationResult(raw_t)) => unit=?,
      ~optimisticResponse: t=?,
      (module Types.Operation with
         type t = t and
         type Raw.t = raw_t and
         type Raw.t_variables = raw_t_variables)
    ) =>
    (mutation(t, raw_t, raw_t_variables), controlledResult(t)) =
  (
    ~client=?,
    ~refetchQueries=?,
    ~awaitRefetchQueries=?,
    ~update=?,
    ~optimisticResponse=?,
    (module Operation),
  ) => {
    let (jsMutate, jsResult) =
      useMutationJs(.
        gql(. Operation.query),
        options(
          ~client?,
          ~refetchQueries?,
          ~awaitRefetchQueries?,
          ~update?,
          ~optimisticResponse=?
            switch (optimisticResponse) {
            | Some(optimisticResponse) =>
              Some(Operation.serialize(optimisticResponse))
            | None => None
            },
          (),
        ),
      );

    let mutate =
      React.useMemo1(
        (
          (),
          ~client=?,
          ~refetchQueries=?,
          ~awaitRefetchQueries=?,
          ~optimisticResponse=?,
          variables,
        ) =>
          jsMutate(.
            options(
              ~variables,
              ~client?,
              ~refetchQueries?,
              ~awaitRefetchQueries?,
              ~optimisticResponse=?
                switch (optimisticResponse) {
                | Some(optimisticResponse) =>
                  Some(Operation.serialize(optimisticResponse))
                | None => None
                },
              (),
            ),
          )
          |> Js.Promise.then_(jsResult => {
               let full =
                 Execution.{
                   data:
                     Js.Nullable.toOption(jsResult##data)
                     ->Belt.Option.map(Operation.parse),
                   errors:
                     switch (Js.Nullable.toOption(jsResult##errors)) {
                     | Some(errors) when Js.Array.length(errors) > 0 =>
                       Some(errors)
                     | _ => None
                     },
                 };

               let simple: Execution.executionVariantResult(Operation.t) =
                 switch (full) {
                 | {errors: Some(errors)} => Errors(errors)
                 | {data: Some(data)} => Data(data)

                 | {errors: None, data: None} => NoData
                 };

               (simple, full) |> Js.Promise.resolve;
             }),
        [||],
      );

    let full =
      React.useMemo1(
        () =>
          {
            loading: jsResult##loading,
            called: jsResult##called,
            data:
              jsResult##data
              ->Js.Nullable.toOption
              ->Belt.Option.map(Operation.parse),
            error: jsResult##error->Js.Nullable.toOption,
          },
        [|jsResult|],
      );

    (mutate, full);
  };

let useMutationWithVariables:
  type t raw_t raw_t_variables.
    (
      ~client: ApolloClient_Client.t=?,
      ~refetchQueries: Execution.refetchQueries(raw_t)=?,
      ~awaitRefetchQueries: bool=?,
      ~update: (ApolloClient_Client.t, mutationResult(raw_t)) => unit=?,
      ~optimisticResponse: t=?,
      ~variables: raw_t_variables,
      (module Types.Operation with
         type t = t and
         type Raw.t = raw_t and
         type Raw.t_variables = raw_t_variables)
    ) =>
    (mutationNoVars(t, raw_t, raw_t_variables), controlledResult(t)) =
  (
    ~client=?,
    ~refetchQueries=?,
    ~awaitRefetchQueries=?,
    ~update=?,
    ~optimisticResponse=?,
    ~variables,
    (module Operation),
  ) => {
    let (jsMutate, jsResult) =
      useMutationJs(.
        gql(. Operation.query),
        options(
          ~client?,
          ~variables,
          ~refetchQueries?,
          ~awaitRefetchQueries?,
          ~update?,
          ~optimisticResponse=?
            switch (optimisticResponse) {
            | Some(optimisticResponse) =>
              Some(Operation.serialize(optimisticResponse))
            | None => None
            },
          (),
        ),
      );

    let mutate = () =>
      jsMutate(. options())
      |> Js.Promise.then_(jsResult => {
           let full =
             Execution.{
               data:
                 Js.Nullable.toOption(jsResult##data)
                 ->Belt.Option.map(Operation.parse),
               errors:
                 switch (Js.Nullable.toOption(jsResult##errors)) {
                 | Some(errors) when Js.Array.length(errors) > 0 =>
                   Some(errors)
                 | _ => None
                 },
             };

           let simple: Execution.executionVariantResult(Operation.t) =
             switch (full) {
             | {errors: Some(errors)} => Errors(errors)
             | {data: Some(data)} => Data(data)

             | {errors: None, data: None} => NoData
             };

           (simple, full) |> Js.Promise.resolve;
         });

    let full =
      React.useMemo1(
        () =>
          {
            loading: jsResult##loading,
            called: jsResult##called,
            data:
              jsResult##data
              ->Js.Nullable.toOption
              ->Belt.Option.map(Operation.parse),
            error: jsResult##error->Js.Nullable.toOption,
          },
        [|jsResult|],
      );

    (mutate, full);
  };

let useMutation0:
  type t raw_t raw_t_variables.
    (
      ~client: ApolloClient_Client.t=?,
      ~refetchQueries: Execution.refetchQueries(raw_t)=?,
      ~awaitRefetchQueries: bool=?,
      ~update: (ApolloClient_Client.t, mutationResult(raw_t)) => unit=?,
      ~optimisticResponse: t=?,
      (module Types.OperationNoRequiredVars with
         type t = t and
         type Raw.t = raw_t and
         type Raw.t_variables = raw_t_variables)
    ) =>
    (mutation0(t, raw_t, raw_t_variables), controlledResult(t)) =
  (
    ~client=?,
    ~refetchQueries=?,
    ~awaitRefetchQueries=?,
    ~update=?,
    ~optimisticResponse=?,
    (module Operation),
  ) => {
    let (mutate, full) =
      useMutation(
        ~client?,
        ~refetchQueries?,
        ~awaitRefetchQueries?,
        ~update?,
        ~optimisticResponse?,
        (module Operation),
      );
    let mutate =
        (
          ~client=?,
          ~refetchQueries=?,
          ~awaitRefetchQueries=?,
          ~optimisticResponse=?,
          ~variables=?,
          (),
        ) => {
      mutate(
        ~client?,
        ~refetchQueries?,
        ~awaitRefetchQueries?,
        ~optimisticResponse?,
        switch (variables) {
        | Some(variables) => variables
        | None => Operation.makeDefaultVariables()
        },
      );
    };
    (mutate, full);
  };

let useMutationLegacy:
  type t raw_t raw_t_variables.
    (
      ~client: ApolloClient_Client.t=?,
      ~refetchQueries: Execution.refetchQueries(raw_t)=?,
      ~awaitRefetchQueries: bool=?,
      ~update: (ApolloClient_Client.t, mutationResult(raw_t)) => unit=?,
      ~optimisticResponse: t=?,
      (module Types.Operation with
         type t = t and
         type Raw.t = raw_t and
         type Raw.t_variables = raw_t_variables)
    ) =>
    (
      mutation(t, raw_t, raw_t_variables),
      controlledVariantResult(t),
      controlledResult(t),
    ) =
  (
    ~client=?,
    ~refetchQueries=?,
    ~awaitRefetchQueries=?,
    ~update=?,
    ~optimisticResponse=?,
    (module Operation),
  ) => {
    let (mutate, result) =
      useMutation(
        ~client?,
        ~refetchQueries?,
        ~awaitRefetchQueries?,
        ~update?,
        ~optimisticResponse?,
        (module Operation),
      );
    let simple =
      React.useMemo1(
        () =>
          switch (result) {
          | {loading: true} => Loading
          | {error: Some(error)} => Error(error)
          | {data: Some(data)} => Data(data)
          | {called: false} => NotCalled
          | _ => NoData
          },
        [|result|],
      );

    (mutate, simple, result);
  };

module Extend = (M: Types.Operation) => {
  let use =
      (
        ~client=?,
        ~refetchQueries=?,
        ~awaitRefetchQueries=?,
        ~update=?,
        ~optimisticResponse=?,
        (),
      ) => {
    useMutation(
      ~client?,
      ~refetchQueries?,
      ~awaitRefetchQueries?,
      ~update?,
      ~optimisticResponse?,
      (module M),
    );
  };
  let useWithVariables =
      (
        ~client=?,
        ~refetchQueries=?,
        ~awaitRefetchQueries=?,
        ~update=?,
        ~optimisticResponse=?,
        variables,
      ) => {
    useMutationWithVariables(
      ~client?,
      ~refetchQueries?,
      ~awaitRefetchQueries?,
      ~update?,
      ~optimisticResponse?,
      ~variables,
      (module M),
    );
  };
};

module ExtendNoRequiredVars = (M: Types.OperationNoRequiredVars) => {
  /**
  Use hook for apollo client
  */
  let use =
      (
        ~client=?,
        ~refetchQueries=?,
        ~awaitRefetchQueries=?,
        ~update=?,
        ~optimisticResponse=?,
        (),
      ) => {
    useMutation(
      ~client?,
      ~refetchQueries?,
      ~awaitRefetchQueries?,
      ~update?,
      ~optimisticResponse?,
      (module M),
    );
  };
  let useWithVariables =
      (
        ~client=?,
        ~refetchQueries=?,
        ~awaitRefetchQueries=?,
        ~update=?,
        ~optimisticResponse=?,
        variables,
      ) => {
    useMutationWithVariables(
      ~client?,
      ~refetchQueries?,
      ~awaitRefetchQueries?,
      ~update?,
      ~optimisticResponse?,
      ~variables,
      (module M),
    );
  };
};
