open ApolloHooks;

let filterAgeLimit = 42;
let filterName = "Bob";

[@react.component]
let make = () => {
  let editPersonRefetchQueries = _ => {
    let query =
      FilterByAge.PersonsOlderThanQuery.make(~age=filterAgeLimit, ());
    [|toQueryObj(query)|];
  };

  let editPersonUpdate = (client, mutationResult) => {
    let data =
      mutationResult##data
      ->Belt.Option.flatMap(result => result##updatePerson);
    switch (data) {
    | Some(person) =>
      FilterByNameCache.updateCache(client, person, filterName)
    | None => ()
    };
  };

  <>
    <div className="edit-person-container">
      <div className="edit-person">
        <EditPerson
          refetchQueries=editPersonRefetchQueries
          update=editPersonUpdate
        />
        <FilterByAge age=filterAgeLimit />
        <FilterByNameCache name=filterName />
      </div>
    </div>
    <Persons />
    <div className="add-person-container">
      <div className="add-person"> <AddPerson /> </div>
    </div>
    <SubscribeToMore />
  </>;
};
