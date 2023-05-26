function gen_fields() {
    var oll = document.getElementById("options03");
    var oll1 = document.getElementById("options13");
    var count = document.getElementById("count_options3").value;
    oll.parentNode.removeChild(oll);
    oll1.parentNode.removeChild(oll1);
    var list = document.createElement("ol");
    list.id = "options03";
    list.type = "A";
    for(let i = 0; i < count; i++){
        var one_input_block = document.createElement("li");
        var input = document.createElement("input");
        input.type = "text";
        input.id = "input_A_" + i;
        one_input_block.appendChild(input);
        list.appendChild(one_input_block);
    }
    document.getElementById('frame').appendChild(list);

    var list1 = document.createElement("ol");
    list1.id = "options13";
    list1.type = "1";
    for(let i1 = 0; i1 < count; i1++){
        var one_input_block1 = document.createElement("li");
        var input1 = document.createElement("input");
        input1.type = "text";
        input1.id = "input_1_" + i1;
        one_input_block1.appendChild(input1);
        list1.appendChild(one_input_block1);
    }
    document.getElementById('frame').appendChild(list1);
}
function set_fields(count) {
    var oll = document.getElementById("options03");
    var oll1 = document.getElementById("options13");
    oll.parentNode.removeChild(oll);
    oll1.parentNode.removeChild(oll1);
    var list = document.createElement("ol");
    list.id = "options03";
    list.type = "A";
    for(let i = 0; i < count; i++){
        var one_input_block = document.createElement("li");
        var input = document.createElement("input");
        input.type = "text";
        input.id = "input_A_" + i;
        one_input_block.appendChild(input);
        list.appendChild(one_input_block);
    }
    document.getElementById('frame').appendChild(list);

    var list1 = document.createElement("ol");
    list1.id = "options13";
    list1.type = "1";
    for(let i1 = 0; i1 < count; i1++){
        var one_input_block1 = document.createElement("li");
        var input1 = document.createElement("input");
        input1.type = "text";
        input1.id = "input_1_" + i1;
        one_input_block1.appendChild(input1);
        list1.appendChild(one_input_block1);
    }
    document.getElementById('frame').appendChild(list1);
}
