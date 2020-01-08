open ApolloHooks;
module EditPersonMutation = [%graphql
  {|
    mutation updatePerson($id: ID!, $age: Int!, $name: String!) {
      updatePerson(id: $id, age: $age, name: $name) {
          id
          age
          name
        }
      }
  |}
];

module OptimisticResponse = {
  /* We need to manually serialise the mutation response into an object that apollo-client understands.
   * See the docs here https://www.apollographql.com/docs/react/performance/optimistic-ui/
   *
   * There is a PR at graphql_ppx_re (https://github.com/baransu/graphql_ppx_re/pull/20) that adds a
   * serialisation function directly to the generated mutation. That should make this step unnecessary for most
   * usecases.
   */
  type t = {
    .
    "__typename": string,
    "updatePerson": {
      .
      "__typename": string,
      "age": int,
      "id": string,
      "name": string,
    },
  };

  external cast: t => Js.Json.t = "%identity";

  let make = (~id, ~name, ~age) =>
    {
      "__typename": "Mutation",
      "updatePerson": {
        "__typename": "Person",
        "id": id,
        "name": name,
        "age": age,
      },
    }
    ->cast;
};

type state = {
  id: string,
  age: option(int),
  name: string,
};

type action =
  | SetId(string)
  | SetAge(option(int))
  | SetName(string);

let reducer = (state, action) => {
  switch (action) {
  | SetId(id) => {...state, id}
  | SetAge(age) => {...state, age}
  | SetName(name) => {...state, name}
  };
};

[@react.component]
let make = (~refetchQueries, ~update) => {
  let (state, dispatch) =
    React.useReducer(reducer, {age: None, name: "", id: ""});

  let (editPersonMutation, _simple, _full) =
    useMutation(~refetchQueries, ~update, EditPersonMutation.definition);

  let handleSubmit = event => {
    ReactEvent.Form.preventDefault(event);
    switch (state.age) {
    | Some(age) =>
      editPersonMutation(
        ~variables=
          EditPersonMutation.makeVariables(
            ~age,
            ~id=state.id,
            ~name=state.name,
            (),
          ),
        ~optimisticResponse=
          OptimisticResponse.make(~id=state.id, ~name=state.name, ~age),
        (),
      )
      |> ignore
    | None => ignore()
    };
  };

  <form onSubmit=handleSubmit>
    <h2> {React.string("Update person by id")} </h2>
    <p> {React.string("Copy the Id of a person from the list to update")} </p>
    <div className="form-field">
      <input
        required=true
        placeholder="Id"
        value={state.id}
        onChange={event => {
          let value = event->ReactEvent.Form.target##value;
          dispatch(SetId(value));
        }}
      />
    </div>
    <div className="form-field">
      <input
        placeholder="Name"
        required=true
        value={state.name}
        onChange={event => {
          let value = event->ReactEvent.Form.target##value;
          dispatch(SetName(value));
        }}
      />
    </div>
    <div className="form-field">
      <input
        required=true
        pattern="\\d{1,3}"
        placeholder="Age"
        value={
          state.age
          ->Belt.Option.map(string_of_int)
          ->Belt.Option.getWithDefault("")
        }
        onChange={event => {
          let value = event->ReactEvent.Form.target##value;
          switch (int_of_string(value)) {
          | value => dispatch(SetAge(Some(value)))
          | exception _ => dispatch(SetAge(None))
          };
        }}
      />
    </div>
    <div className="form-field">
      <input type_="submit" value="Update person" />
    </div>
  </form>;
};
