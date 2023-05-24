function gen_fields() {
    const old_body_table = document.getElementById("tableBody");
    const new_body_table = document.createElement("tbody");
    new_body_table.id = "tableBody";
    old_body_table.parentNode.replaceChild(new_body_table, old_body_table);


    var count_cols = document.getElementById("count_cols3").value;
    var count_rows = document.getElementById("count_rows3").value;
    const o_table = document.getElementById("tableBody");
    for(let i = 0; i < count_rows; i++){
        var one_input_block = document.createElement("tr");
        for(let j = 0; j < count_cols; j++){
            var input = document.createElement("input");
            var td_el = document.createElement("td");
            input.type = "text";
            input.id = "input_" + i + j;
            input.className = "table_field";
            td_el.appendChild(input);
            one_input_block.appendChild(td_el);
        }
        o_table.appendChild(one_input_block);
    }
}
