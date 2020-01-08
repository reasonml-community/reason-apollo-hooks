open ApolloHooks;

module EditPersonMutation = [%graphql
  {|
    mutation addPerson($age: Int!, $name: String!) {
      createPerson(age: $age, name: $name) {
        id
        age
        name
      }
    }
  |}
];

[@react.component]
let make = () => {
  let (name, setName) = React.useState(_ => "");
  let (age, setAge) = React.useState(_ => 0);

  let (addPersonMutation, _simple, _full) =
    useMutation(EditPersonMutation.definition);

  let handleSubmit = event => {
    ReactEvent.Form.preventDefault(event);
    addPersonMutation(
      ~variables=EditPersonMutation.makeVariables(~age, ~name, ()),
      (),
    )
    |> ignore;
  };

  <form onSubmit=handleSubmit>
    <h2> {React.string("Add a new person")} </h2>
    <div className="form-field">
      <input
        placeholder="Name"
        required=true
        value=name
        onChange={event => {
          let value = event->ReactEvent.Form.target##value;
          setName(value);
        }}
      />
    </div>
    <div className="form-field">
      <input
        required=true
        pattern="\\d{1,3}"
        placeholder="Age"
        value={age->string_of_int}
        onChange={event => {
          let value = event->ReactEvent.Form.target##value;
          switch (int_of_string(value)) {
          | intValue => setAge(_ => intValue)
          | exception _ => setAge(_ => 0)
          };
        }}
      />
    </div>
    <div className="form-field">
      <input type_="submit" value="Add person" />
    </div>
  </form>;
};
