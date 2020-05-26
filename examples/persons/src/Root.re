open ApolloHooks;

let filterAgeLimit = 42;
let filterName = "Bob";

type example =
  | LoadMore
  | LoadMoreFragments
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
      <button
        className={getTabClassName(LoadMoreFragments)}
        onClick={_ => setActiveExample(_ => LoadMoreFragments)}>
        {React.string("Load More Fragments")}
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
               <h3> {React.string("FilterByAge.re")} </h3>
               <FilterByAge age=filterAgeLimit />
               <section>
                 <title>
                   {React.string("FilterByAgeErrorHandling.re")}
                 </title>
                 <FilterByAgeErrorHandling age=filterAgeLimit />
               </section>
               <h3> {React.string("FilterByAgeFragment.re")} </h3>
               <FilterByAgeFragment age=filterAgeLimit />
               <h3> {React.string("FilterByNameCache.re")} </h3>
               <FilterByNameCache name=filterName />
             </div>
           </div>
           <LoadMore />
         </>
       | LoadMoreFragments =>
         <>
           <div className="edit-person-container">
             <div className="edit-person">
               <EditPerson
                 refetchQueries=editPersonRefetchQueries
                 update=editPersonUpdate
               />
               <h3> {React.string("FilterByAge.re")} </h3>
               <FilterByAge age=filterAgeLimit />
               <section>
                 <title>
                   {React.string("FilterByAgeErrorHandling.re")}
                 </title>
                 <FilterByAgeErrorHandling age=filterAgeLimit />
               </section>
               <h3> {React.string("FilterByAgeFragment.re")} </h3>
               <FilterByAgeFragment age=filterAgeLimit />
               <h3> {React.string("FilterByNameCache.re")} </h3>
               <FilterByNameCache name=filterName />
             </div>
           </div>
           <LoadMoreFragments />
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