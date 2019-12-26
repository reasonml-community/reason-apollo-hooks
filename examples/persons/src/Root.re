open ApolloHooks;

let filterAgeLimit = 42;
let filterName = "Bob";

type example =
  | LoadMore
  | SubscribeToMore;

[@react.component]
let make = () => {
  let (activeExample, setActiveExample) = React.useState(_ => LoadMore);

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

  let getTabClassName = tabExample =>
    "tab" ++ (tabExample == activeExample ? " selected-tab" : "");

  <>
    <div className="tabs">
      <button
        className={getTabClassName(LoadMore)}
        onClick={_ => setActiveExample(_ => LoadMore)}>
        {React.string("Load More")}
      </button>
      <button
        className={getTabClassName(SubscribeToMore)}
        onClick={_ => setActiveExample(_ => SubscribeToMore)}>
        {React.string("Subscribe to More")}
      </button>
    </div>
    <div className="tab-content">
      {switch (activeExample) {
       | LoadMore =>
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
           <LoadMore />
         </>
       | SubscribeToMore =>
         <>
           <div className="add-person-container">
             <div className="add-person"> <AddPerson /> </div>
           </div>
           <SubscribeToMore />
         </>
       }}
    </div>
  </>;
};
