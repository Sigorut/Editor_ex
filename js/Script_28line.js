function set_fields(codogenic_chain, transcribed_chain, iRNA_chain, polypeptide) {
    let text_data = "Фрагмент молекулы ДНК имеет следующую последовательность нуклеотидов (верхняя цепь  — смысловая, нижняя  — транскрибируемая):<br><br>";
    text_data += "5'- " + codogenic_chain + " -3'<br>" ;
    text_data += "3'- " + transcribed_chain + " -5'";
    let text_ex = "Определите последовательность аминокислот во фрагменте полипептидной цепи.";
    let text_ex_field = document.getElementById("text_ex");
    text_ex_field.innerHTML = text_ex;
    let text_data_field = document.getElementById("text_data");
    text_data_field.innerHTML = text_data;
    let answer_iRNA = document.getElementById("answer_iRNA");
    answer_iRNA.value = iRNA_chain;
    let answer_poly = document.getElementById("answer_poly");
    answer_poly.value = polypeptide;
}
