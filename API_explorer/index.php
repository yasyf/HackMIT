<?php
	session_start();
	$_SESSION['n'] = 1;
	if(isset($_REQUEST['n']) && isset($_REQUEST['endpoint']) && isset($_REQUEST['method'])){
		$url = $_SESSION['endpoint']."/".$_SESSION['method'];
		$post_data = array();
		foreach ($_REQUEST as $key => $value) {
			$_SESSION[$key] = $value;
			if(strpos($key,"k") === 0){
				$post_data[$value] = $_REQUEST["v".substr($key,1)];
			}
			
		}
		if($_REQUEST['submit'] == "post"){
			
			$ch = curl_init($url);
			curl_setopt($ch, CURLOPT_POST, 1);
			curl_setopt($ch, CURLOPT_POSTFIELDS, $post_data);
			curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
			$response = curl_exec($ch);
			curl_close($ch);
		}
		else if ($_REQUEST['submit'] == "get") {
			$url = $url."?".http_build_query($post_data);
			$response = file_get_contents($url);
		}
	}
?>

<!DOCTYPE html>
<html lang="en">
<head>
<title>API Explorer</title>
<script src="//code.jquery.com/jquery-1.10.1.min.js"></script>
<link href="//netdna.bootstrapcdn.com/bootstrap/3.0.0/css/bootstrap.min.css" rel="stylesheet">
<script src="//netdna.bootstrapcdn.com/bootstrap/3.0.0/js/bootstrap.min.js"></script>
</head>
<body>
 <div class="container">

      <div class="starter-template">
        <h1>API Explorer</h1>
        <p class="lead"></p>

<form action=""  role="form" id="form" method="POST">
<div class="form-group">
<input type="text" name="endpoint" placeholder="Endpoint" value="<?php echo $_SESSION['endpoint']; ?>">/<input type="text" name="method" placeholder="Method" value="<?php echo $_SESSION['method']; ?>">
</div>
<br />
<div class="form-group">
<input type="text" name="k1" value="<?php echo $_SESSION['k1']; ?>" placeholder="Key">&nbsp;:&nbsp;<input type="text" name="v1" value="<?php echo $_SESSION['v1']; ?>" placeholder="Value">
</div>
<input type="hidden" id="ncounter" name="n" value="<?php echo $_SESSION['n']; ?>"><br />
<div id="toadd">
<?php
for($i=2;$i<=$_SESSION['n'];$i++){
	echo '<div class="form-group"><input type="text" name="k'.$i.'" value="'.$_SESSION["k$i"].'" placeholder="Key">&nbsp;/&nbsp;<input type="text" name="v'.$i.'" value="'.$_SESSION["v$i"].'" placeholder="Value"></div>';
}

?>
</div>
<div class="form-group">
<button type="submit" name="submit" value="post">POST</button>
<button type="submit" name="submit" value="get">GET</button>
</div>
</form>
<button type="button" id="add">Add Paramater</button>
<br /><br /><br /><br />
<div id="results" style="border:2px;width:100%;height:500px;overflow:scroll;">
<pre>
<?php 

if($_REQUEST['submit'] == "post"){
	echo "Executing POST request to $url\n\n";
	print_r($post_data);
}
else if ($_REQUEST['submit'] == "get") {
	echo "Executing GET request to $url\n\n";
}

echo $response; 

?>
</pre>
</div>
      </div>

    </div><!-- /.container -->
<script>
var counter = <?php echo $_SESSION['n']; ?>;
$("#add").click(function(){
	counter++;
	$("#ncounter").val(counter);
	$("#toadd").append('<div class="form-group"><input type="text" name="k'+counter+'" placeholder="Key">&nbsp;/&nbsp;<input type="text" name="v'+counter+'" placeholder="Value"></div>');
})
</script>			
</body>
</html>