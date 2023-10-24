window.onload = () => {
  $("#sendbutton").click(() => {
    imagebox = $("#imagebox");
    link = $("#link");
    input = $("#imageinput")[0];
    
    if (input.files && input.files[0]) {
      let formData = new FormData();
      formData.append("video", input.files[0]);
      $.ajax({
        url: "/detect", // fix this to your liking
        type: "POST",
        data: formData,
        cache: false,
        processData: false,
        contentType: false,
        error: function (data) {
          console.log("upload error", data);
          console.log(data.getAllResponseHeaders());
        },
        success: function (data) {
          console.log(data);
          //sortie.attr("src","runs/detect/exp/" + data ).height(500).width(500); 
          
          $("#imageinput2").attr("src", data[0]); 
          document.getElementById("num1").innerHTML = data[1];
          document.getElementById("num2").innerHTML = data[2];
          document.getElementById("numadv").innerHTML = data[3];
          document.getElementById("numret").innerHTML = data[4];
          document.getElementById("numdead").innerHTML = data[5];
          document.getElementById("numtot").innerHTML = data[6]



          var xValues = ["STAGE1", "STAGE2-4","ADVANCED" , "RETURNED", "DEAD", "TOTAL"];
          var yValues = [data[1], data[2], data[3], data[4], data[5], data[6]];
          var barColors = [
            "#b91d47",
            "#0000FF",
            '#808080',
            "#00FF00",
            "#FF00FF",
            "#00FFFF"
          ];
          
          new Chart("myChart", {
            type: "pie",
            data: {
              labels: xValues,
              datasets: [{
                backgroundColor: barColors,
                data: yValues
              }]
            },
            options: {
              title: {
                display: true,
                text: "Egg balance"
              }
            }
          });

          new Chart(document.getElementById("bar-chart"), {
            type: 'bar',
            data: {
              labels: ["STAGE1", "STAGE2-4","ADVANCED" , "RETURNED", "DEAD", "TOTAL"],
              datasets: [
                {
                  label: "eggs",
                  backgroundColor: ["#b91d47", "#0000FF", '#808080',"#00FF00","#FF00FF","#00FFFF"],
                  data: [data[1],  data[2], data[3],data[4], data[5], data[6]]
                }
              ]
            },
            options: {
              legend: { display: false },
              title: {
                display: true,
                text: 'Population of eggs'
              }
            }
        });


          

          

        },
      });
    }
  });

  $("#opencam").click(() => {
    console.log("evoked openCam");
    $.ajax({
      url: "/opencam",
      type: "GET",
      error: function (data) {
        console.log("upload error", data);
      },
      success: function (data) {
        console.log(data);

        $("#imageinput3").attr("src", data); 

          
          
      }
    });
  })
};

function readUrl(input) {
  imagebox = $("#imageinput1");
  console.log(imagebox);
  console.log("evoked readUrl");
  if (input.files && input.files[0]) {
    var reader = new FileReader();
    reader.onload = function (e) {
      console.log(e.target);

      imagebox.attr("src", e.target.result); 

      
       //imagebox.height(500);
      //imagebox.width(500);
    };
    reader.readAsDataURL(input.files[0]);
  }
}


function openCam(e){
  console.log("evoked openCam");
  e.preventDefault();
  console.log("evoked openCam");
  console.log(e);
}
