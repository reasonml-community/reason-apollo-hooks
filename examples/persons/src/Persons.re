[@react.component]
let make = (~persons) =>
  <div className="person-list">
    {persons->Belt.Array.map(person =>
       <div key=person##id className="person">
         <div className="person-field">
           <span className="person-label"> {React.string("Id: ")} </span>
           {React.string(person##id)}
         </div>
         <div className="person-field">
           <span className="person-label"> {React.string("Name: ")} </span>
           {React.string(person##name)}
         </div>
         <div className="person-field">
           <span className="person-label"> {React.string("Age: ")} </span>
           {person##age |> string_of_int |> React.string}
         </div>
       </div>
     )
     |> React.array}
  </div>;
