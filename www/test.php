<?php //ghjg

$f = fopen( 'php://stdin', 'r' );

while( $line = fgets( $f ) ) {
  echo $line;
  break ;
}

fclose( $f );

?>


<?php
if ($_SERVER['REQUEST_METHOD'] == 'POST') {
	$f = fopen( 'php://stdin', 'r' );

	while( $line = fgets( $f ) ) {
	  echo $line;
	  //break ;
	}
	
	fclose( $f );

   
} 
?>
<!DOCTYPE html>
<div>
    <?php if (isset($_COOKIE['name']) && isset($_COOKIE['email'])): ?>
        <p>
            <center><h1>Hello, <?= $_COOKIE['name'] ?>!</h1></center><hr>
            <center><img src="<?= $_COOKIE['avatar'] ?>" alt="avatar" width="300" height="300" style="object-fit: cover;"></center><hr><br/>
            <center><?php echo $_COOKIE['name']; ?></center>
            <center><?php echo $_COOKIE['email']; ?></center><br/><br/>
            <center><a href="?logout">Logout</a></center>
        </p>
    <?php else: ?>
        <form method="post" enctype="multipart/form-data">
            <input type="text" name="name" placeholder="Name" />
            <input type="text" name="email" placeholder="Email" />
            <input type="file" id="avatar" name="avatar" accept="image/png, image/jpeg" />
            <input type="submit" value="Submit" />
        </form>
    <?php endif; ?>
</div>