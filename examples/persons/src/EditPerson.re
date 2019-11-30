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
let make = () => {
  let filterAgeLimit = 42;
  let filterName = "Bob";

  let (state, dispatch) =
    React.useReducer(reducer, {age: None, name: "", id: ""});

  let (editPersonMutation, _simple, _full) =
    useMutation(
      ~refetchQueries=
        _ => {
          let query =
            FilterByAge.PersonsOlderThanQuery.make(~age=filterAgeLimit, ());
          [|toQueryObj(query)|];
        },
      ~update=
        (client, mutationResult) => {
          let data =
            mutationResult##data
            ->Belt.Option.flatMap(result => result##updatePerson);
          switch (data) {
          | Some(person) =>
            FilterByNameCache.updateCache(client, person, filterName)
          | None => ()
          };
        },
      EditPersonMutation.definition,
    );

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
        (),
      )
      |> ignore
    | None => ignore()
    };
  };

  <div className="edit-person-container">
    <div className="edit-person">
      <form onSubmit=handleSubmit>
        <h2> {React.string("Update person by id")} </h2>
        <p>
          {React.string("Copy the Id of a person from the list to update")}
        </p>
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
      </form>
      <FilterByAge age=filterAgeLimit />
      <FilterByNameCache name=filterName />
    </div>
  </div>;
};
