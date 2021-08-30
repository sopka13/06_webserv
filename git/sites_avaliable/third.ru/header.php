<?php
/**
 * The header for our theme.
 *
 * This is the template that displays all of the <head> section and everything up until <div id="content">
 *
 * @link https://developer.wordpress.org/themes/basics/template-files/#template-partials
 *
 * @package BeOnePage
 */

?><!DOCTYPE html>
<html <?php language_attributes(); ?>>
<head>
<meta charset="<?php bloginfo( 'charset' ); ?>">
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="profile" href="http://gmpg.org/xfn/11">
<link rel="pingback" href="<?php bloginfo( 'pingback_url' ); ?>">

<?php wp_head(); ?>
</head>

<body <?php body_class(); ?>>
<div id="page" class="hfeed site">
	<a class="skip-link sr-only" href="#content"><?php esc_html_e( 'Skip to content', 'beonepage' ); ?></a>

	<header id="masthead" class="site-header <?php echo Kirki::get_option( 'general_sticky_menu' ) == '1' ? 'sticky' : 'no-sticky'; ?>" role="banner">
		<div class="container">
			<div class="row">
				<div class="col-md-12 clearfix">
					<div class="site-branding">
						<h1 class="site-title"><a href="<?php echo esc_url( home_url( '/' ) ); ?>" rel="home"><?php bloginfo( 'name' ); ?></a></h1>
					</div><!-- .site-branding -->

					<span id="mobile-menu" class="mobile-menu"></span>

					<nav id="site-navigation" class="main-navigation" role="navigation">
						<?php
							wp_nav_menu( array(
								'menu'            => 'primary',
								'theme_location'  => 'primary',
								'menu_id'         => 'primary-menu',
								'menu_class'      => 'menu clearfix',
								'container'       => false,
								'depth'           => 2,
								'walker'          => new beonepage_Walker_Nav_Menu,
								'fallback_cb'     => 'beonepage_Walker_Nav_Menu::fallback'
								)
							);
						?>
					</nav><!-- #site-navigation -->
				</div><!-- .col-md-12 -->
			</div><!-- .row -->
		</div><!-- .container -->
	</header><!-- #masthead -->

	<div id="content" class="site-content">
