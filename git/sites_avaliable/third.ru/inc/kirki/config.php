<?php
/**
 * Kirki Advanced Customizer
 *
 * @category BeOnePage
 * @package Kirki
 * @link    https://github.com/aristath/kirki
 */

/**
 * Remover or change default sections for Theme Customizer.
 */
function beonepage_remove_customize_section( $wp_customize ) {
	$wp_customize->remove_section( 'colors' );
	$wp_customize->remove_section( 'title_tagline' );
	$wp_customize->remove_section( 'static_front_page' );

	$wp_customize->get_panel( 'nav_menus' )->priority = 25;
	$wp_customize->get_control( 'site_icon' )->priority = 5;
	$wp_customize->get_control( 'site_icon' )->section = 'site_icon_logo';
}
add_action( 'customize_register', 'beonepage_remove_customize_section', 1000 );

// Early exit if Kirki is not installed
if ( ! class_exists( 'Kirki' ) ) {
	return;
}

/**
 * Create panels using the Kirki API.
 */
Kirki::add_section( 'site_general', array(
	'priority'    => 10,
	'title'       => __( 'General', 'beonepage' ),
	'description' => __( 'You may need to refresh the page to see the changes.', 'beonepage' ),
) );

Kirki::add_section( 'site_icon_logo', array(
	'priority'    => 20,
	'title'       => __( 'Site Icon', 'beonepage' ),
	'description' => __( 'Here you can upload your own custom favicon image', 'beonepage' )
) );

/* Front Page Panel Start */
Kirki::add_panel( 'site_front_page', array(
	'priority'        => 30,
	'title'           => __( 'Front Page', 'beonepage' ),
	'active_callback' => 'is_front_page'
) );

Kirki::add_section( 'site_front_page_slider_module', array(
	'priority' => 31,
	'title'    => __( 'Slider Module', 'beonepage' ),
	'panel'    => 'site_front_page'
) );

Kirki::add_section( 'site_front_page_icon_service_module', array(
	'priority' => 32,
	'title'    => __( 'Icon Service Module', 'beonepage' ),
	'panel'    => 'site_front_page'
) );

Kirki::add_section( 'site_front_page_portfolio_module', array(
	'priority' => 33,
	'title'    => __( 'Portfolio Module', 'beonepage' ),
	'panel'    => 'site_front_page'
) );

Kirki::add_section( 'site_front_page_ver_promo_module', array(
	'priority' => 34,
	'title'    => __( 'Vertical Promotion Module', 'beonepage' ),
	'panel'    => 'site_front_page'
) );

Kirki::add_section( 'site_front_page_blog_module', array(
	'priority' => 35,
	'title'    => __( 'Blog Module', 'beonepage' ),
	'panel'    => 'site_front_page'
) );

Kirki::add_section( 'site_front_page_contact_module', array(
	'priority' => 36,
	'title'    => __( 'Contact Module', 'beonepage' ),
	'panel'    => 'site_front_page'
) );
/* Front Page Panel End */

Kirki::add_section( 'site_blog_page', array(
	'priority'    => 40,
	'title'       => __( 'Blog Page', 'beonepage' ),
	'description' => __( 'Here you can customize your blog pages.', 'beonepage' ),
	'active_callback' => 'beonepage_is_blog_page'
) );

Kirki::add_section( 'site_footer', array(
	'priority'    => 50,
	'title'       => __( 'Site Footer', 'beonepage' ),
	'description' => __( 'Here you can customize the footer on your site.', 'beonepage' )
) );

Kirki::add_section( 'upgrade_theme', array(
	'priority' => 60,
	'title'    => __( 'Upgrade', 'beonepage' )
) );

/**
 * Create a config instance that will be used by fields added via the static methods.
 */
Kirki::add_config( 'beonepage_kirki', array(
	'option_type' => 'theme_mod'
) );

/**
 * Create fields using the Kirki API static functions.
 */
/* General Section Start */
Kirki::add_field( 'beonepage_kirki', array(
	'type'      => 'text',
	'settings'  => 'general_site_title',
	'label'     => __( 'Site Title', 'beonepage' ),
	'section'   => 'site_general',
	'default'   => get_bloginfo( 'name' ),
	'priority'  => 10,
	'transport' => 'postMessage',
	'js_vars'   => array(
		array(
			'element'  => '.site-title a',
			'function' => 'html',
			'property' => 'text'
		)
    )
) );

Kirki::add_field( 'beonepage_kirki', array(
	'type'     => 'dropdown-pages',
	'settings' => 'general_front_page',
	'label'    => __( 'Front Page', 'beonepage' ),
	'help'     => __( 'Create a blank page with Home Page template and set it as Front Page.', 'beonepage' ),
	'section'  => 'site_general',
	'default'  => get_option( 'page_on_front' ),
	'priority' => 20
) );

Kirki::add_field( 'beonepage_kirki', array(
	'type'     => 'dropdown-pages',
	'settings' => 'general_posts_page',
	'label'    => __( 'Posts Page', 'beonepage' ),
	'help'     => __( 'Create a blank page and set it as Posts Page.', 'beonepage' ),
	'section'  => 'site_general',
	'default'  => get_option( 'page_for_posts' ),
	'priority' => 30
) );

Kirki::add_field( 'beonepage_kirki', array(
	'type'     => 'checkbox',
	'settings' => 'general_sticky_menu',
	'label'    => __( 'Enable Sticky Menu?', 'beonepage' ),
	'help'     => __( 'If enable, the menu will be accessible from anywhere without having to scroll.', 'beonepage' ),
	'section'  => 'site_general',
	'default'  => '1',
	'priority' => 40
) );

Kirki::add_field( 'beonepage_kirki', array(
	'type'     => 'checkbox',
	'settings' => 'general_go_to_top',
	'label'    => __( 'Enable Go to Top Button?', 'beonepage' ),
	'section'  => 'site_general',
	'default'  => '1',
	'priority' => 50
) );
/* General Section End */

/* Front Page Panel Start */
/* Slider Module Section Start */
Kirki::add_field( 'beonepage_kirki', array(
	'type'      => 'text',
	'settings'  => 'front_page_text_slider_title',
	'label'     => __( 'Heading', 'beonepage' ),
	'help'      => __( 'If you want to color the word, just wrap it with \"span\" tag.', 'beonepage' ),
	'section'   => 'site_front_page_slider_module',
	'default'   => 'Be <span>Imaginative</span> &bull; Be <span>Yourself</span>',
	'priority'  => 10,
	'transport' => 'postMessage',
	'js_vars'   => array(
		array(
			'element'  => '.slider .slider-caption h1',
			'function' => 'html',
			'property' => 'text'
		)
    )
) );

Kirki::add_field( 'beonepage_kirki', array(
	'type'      => 'text',
	'settings'  => 'front_page_text_slider_content',
	'label'     => __( 'Content', 'beonepage' ),
	'section'   => 'site_front_page_slider_module',
	'default'   => 'We handcraft well-thought-out WordPress themes',
	'priority'  => 20,
	'transport' => 'postMessage',
	'js_vars'   => array(
		array(
			'element'  => '.slider .slider-caption p',
			'function' => 'html',
			'property' => 'text'
		)
    )
) );

Kirki::add_field( 'beonepage_kirki', array(
	'type'      => 'text',
	'settings'  => 'front_page_text_slider_btn_text',
	'label'     => __( 'Button Text', 'beonepage' ),
	'section'   => 'site_front_page_slider_module',
	'default'   => 'Learn More',
	'priority'  => 30,
	'transport' => 'postMessage',
	'js_vars'   => array(
		array(
			'element'  => '.slider-btn .btn',
			'function' => 'html',
			'property' => 'text'
		)
    )
) );

Kirki::add_field( 'beonepage_kirki', array(
	'type'      => 'text',
	'settings'  => 'front_page_text_slider_btn_url',
	'label'     => __( 'Button URL', 'beonepage' ),
	'section'   => 'site_front_page_slider_module',
	'default'   => 'http://betheme.me',
	'priority'  => 40,
	'transport' => 'postMessage',
	'js_vars'   => array(
		array(
			'element'  => '.slider-btn a',
			'function' => 'attr',
			'property' => 'href'
		)
    )
) );

Kirki::add_field( 'beonepage_kirki', array(
	'type'     => 'background',
	'setting'  => 'front_page_text_slider_bg',
	'label'    => __( 'Background Image', 'beonepage' ),
	'section'  => 'site_front_page_slider_module',
	'default'  => array(
		'image'    => get_template_directory_uri() . '/images/background.jpg',
		'position' => 'center-center'
	),
	'priority' => 50,
	'output'   => '.full-screen'
) );

Kirki::add_field( 'beonepage_kirki', array(
	'type'     => 'checkbox',
	'settings' => 'front_page_text_slider_parallax',
	'label'    => __( 'Enable Parallax Scrolling?', 'beonepage' ),
	'section'  => 'site_front_page_slider_module',
	'default'  => '1',
	'priority' => 60
) );

Kirki::add_field( 'beonepage_kirki', array(
	'type'     => 'checkbox',
	'settings' => 'front_page_text_slider_scroll_down',
	'label'    => __( 'Enable Scroll Down Button?', 'beonepage' ),
	'section'  => 'site_front_page_slider_module',
	'default'  => '1',
	'priority' => 70,
	'js_vars'  => array(
		array(
			'element'  => '.scroll-down',
			'function' => 'css',
			'property' => 'display'
		)
    )
) );
/* Slider Module Section End */

/* Icon Service Module Section Start */
Kirki::add_field( 'beonepage_kirki', array(
	'type'        => 'text',
	'settings'    => 'front_page_icon_service_module_id',
	'label'       => __( 'Module ID', 'beonepage' ),
	'description' => __( 'Set up a unique ID for the module, the ID will be used for page scrolling navigation.', 'beonepage' ),
	'section'     => 'site_front_page_icon_service_module',
	'default'     => 'icon-service-module',
	'priority'    => 10,
	'transport'   => 'postMessage'
) );

Kirki::add_field( 'beonepage_kirki', array(
	'type'      => 'text',
	'settings'  => 'front_page_icon_service_module_title',
	'label'     => __( 'Title', 'beonepage' ),
	'help'      => __( 'If you want to color the word, just wrap it with \"span\" tag.', 'beonepage' ),
	'section'   => 'site_front_page_icon_service_module',
	'default'   => 'Icon <span>Service</span> Module',
	'priority'  => 20,
	'transport' => 'postMessage',
	'js_vars'   => array(
		array(
			'element'  => '.icon-service-module .module-caption h2',
			'function' => 'html',
			'property' => 'text'
		)
    )
) );

Kirki::add_field( 'beonepage_kirki', array(
	'type'      => 'textarea',
	'settings'  => 'front_page_icon_service_module_subtitle',
	'label'     => __( 'Subtitle', 'beonepage' ),
	'section'   => 'site_front_page_icon_service_module',
	'default'   => 'Subtitle for icon service module',
	'priority'  => 30,
	'transport' => 'postMessage',
	'js_vars'   => array(
		array(
			'element'  => '.icon-service-module .module-caption p',
			'function' => 'html',
			'property' => 'text'
		)
    )
) );

Kirki::add_field( 'beonepage_kirki', array(
	'type'        => 'radio-buttonset',
	'settings'    => 'front_page_icon_service_module_layout',
	'label'       => __( 'Layout', 'betheme' ),
	'description' => __( 'Choose the layout for the container.', 'beonepage' ),
	'section'     => 'site_front_page_icon_service_module',
	'default'     => 'fixed',
	'priority'    => 40,
	'choices'     => array(
		'fixed' => __( 'Fixed-width', 'betheme' ),
		'full'  => __( 'Full-width', 'betheme' ),
	),
) );
/* Icon Service Module Section End */

/* Portfolio Module Section Start */
Kirki::add_field( 'beonepage_kirki', array(
	'type'        => 'text',
	'settings'    => 'front_page_portfolio_module_id',
	'label'       => __( 'Module ID', 'beonepage' ),
	'description' => __( 'Set up a unique ID for the module, the ID will be used for page scrolling navigation.', 'beonepage' ),
	'section'     => 'site_front_page_portfolio_module',
	'default'     => 'portfolio-module',
	'priority'    => 10,
	'transport'   => 'postMessage'
) );

Kirki::add_field( 'beonepage_kirki', array(
	'type'      => 'text',
	'settings'  => 'front_page_portfolio_module_title',
	'label'     => __( 'Title', 'beonepage' ),
	'help'      => __( 'If you want to color the word, just wrap it with \"span\" tag.', 'beonepage' ),
	'section'   => 'site_front_page_portfolio_module',
	'default'   => 'Portfolio Module',
	'priority'  => 20,
	'transport' => 'postMessage',
	'js_vars'   => array(
		array(
			'element'  => '.portfolio-module .module-caption h2',
			'function' => 'html',
			'property' => 'text'
		)
    )
) );

Kirki::add_field( 'beonepage_kirki', array(
	'type'      => 'textarea',
	'settings'  => 'front_page_portfolio_module_subtitle',
	'label'     => __( 'Subtitle', 'beonepage' ),
	'section'   => 'site_front_page_portfolio_module',
	'default'   => 'Subtitle for portfolio module',
	'priority'  => 30,
	'transport' => 'postMessage',
	'js_vars'   => array(
		array(
			'element'  => '.portfolio-module .module-caption p',
			'function' => 'html',
			'property' => 'text'
		)
    )
) );

Kirki::add_field( 'beonepage_kirki', array(
	'type'     => 'checkbox',
	'settings' => 'front_page_portfolio_module_filter',
	'label'    => __( 'Enable Portfolio Filter?', 'beonepage' ),
	'section'  => 'site_front_page_portfolio_module',
	'default'  => '1',
	'priority' => 40
) );

Kirki::add_field( 'beonepage_kirki', array(
	'type'      => 'text',
	'settings'  => 'front_page_portfolio_module_all',
	'label'     => __( 'Show All Button Text', 'beonepage' ),
	'section'   => 'site_front_page_portfolio_module',
	'default'   => 'Show All',
	'priority'  => 41,
	'transport' => 'postMessage',
	'js_vars'   => array(
		array(
			'element'  => '#portfolio-filter a:first-child',
			'function' => 'html',
			'property' => 'text'
		)
    ),
	'required' => array(
        array(
            'setting'  => 'front_page_portfolio_module_filter',
            'operator' => '==',
            'value'    => '1'
        )
    )
) );
/* Portfolio Module Section End */

/* Vertical Promotion Module Section Start */
Kirki::add_field( 'beonepage_kirki', array(
	'type'        => 'text',
	'settings'    => 'front_page_ver_promo_module_id',
	'label'       => __( 'Module ID', 'beonepage' ),
	'description' => __( 'Set up a unique ID for the module, the ID will be used for page scrolling navigation.', 'beonepage' ),
	'section'     => 'site_front_page_ver_promo_module',
	'default'     => 'ver-promo-module',
	'priority'    => 10,
	'transport'   => 'postMessage'
) );

Kirki::add_field( 'beonepage_kirki', array(
	'type'      => 'text',
	'settings'  => 'front_page_ver_promo_title',
	'label'     => __( 'Heading', 'beonepage' ),
	'help'      => __( 'If you want to color the word, just wrap it with \"span\" tag.', 'beonepage' ),
	'section'   => 'site_front_page_ver_promo_module',
	'default'   => 'We are <span>BeTheme</span>',
	'priority'  => 20,
	'transport' => 'postMessage',
	'js_vars'   => array(
		array(
			'element'  => '.promo-box-ver-module .promo-box-ver h2',
			'function' => 'html',
			'property' => 'text'
		)
    )
) );

Kirki::add_field( 'beonepage_kirki', array(
	'type'      => 'text',
	'settings'  => 'front_page_ver_promo_content',
	'label'     => __( 'Content', 'beonepage' ),
	'section'   => 'site_front_page_ver_promo_module',
	'default'   => 'We build more than just Themes. We build User Experience for both, you and your visitors.',
	'priority'  => 30,
	'transport' => 'postMessage',
	'js_vars'   => array(
		array(
			'element'  => '.promo-box-ver-module .promo-box-ver p',
			'function' => 'html',
			'property' => 'text'
		)
    )
) );

Kirki::add_field( 'beonepage_kirki', array(
	'type'      => 'text',
	'settings'  => 'front_page_ver_promo_btn_text',
	'label'     => __( 'Button Text', 'beonepage' ),
	'section'   => 'site_front_page_ver_promo_module',
	'default'   => 'Learn More',
	'priority'  => 40,
	'transport' => 'postMessage',
	'js_vars'   => array(
		array(
			'element'  => '.promo-box-ver-module .promo-btn .btn',
			'function' => 'html',
			'property' => 'text'
		)
    )
) );

Kirki::add_field( 'beonepage_kirki', array(
	'type'      => 'text',
	'settings'  => 'front_page_ver_promo_btn_url',
	'label'     => __( 'Button URL', 'beonepage' ),
	'section'   => 'site_front_page_ver_promo_module',
	'default'   => 'http://betheme.me',
	'priority'  => 50,
	'transport' => 'postMessage',
	'js_vars'   => array(
		array(
			'element'  => '.promo-box-ver-module .promo-btn a',
			'function' => 'attr',
			'property' => 'href'
		)
    )
) );
/* Vertical Promotion Module Section End */

/* Blog Module Section Start */
Kirki::add_field( 'beonepage_kirki', array(
	'type'        => 'text',
	'settings'    => 'front_page_blog_module_id',
	'label'       => __( 'Module ID', 'beonepage' ),
	'description' => __( 'Set up a unique ID for the module, the ID will be used for page scrolling navigation.', 'beonepage' ),
	'section'     => 'site_front_page_blog_module',
	'default'     => 'blog-module',
	'priority'    => 10,
	'transport'   => 'postMessage'
) );

Kirki::add_field( 'beonepage_kirki', array(
	'type'      => 'text',
	'settings'  => 'front_page_blog_module_title',
	'label'     => __( 'Title', 'beonepage' ),
	'help'      => __( 'If you want to color the word, just wrap it with \"span\" tag.', 'beonepage' ),
	'section'   => 'site_front_page_blog_module',
	'default'   => 'Blog Module',
	'priority'  => 20,
	'transport' => 'postMessage',
	'js_vars'   => array(
		array(
			'element'  => '.blog-module .module-caption h2',
			'function' => 'html',
			'property' => 'text'
		)
    )
) );

Kirki::add_field( 'beonepage_kirki', array(
	'type'      => 'textarea',
	'settings'  => 'front_page_blog_module_subtitle',
	'label'     => __( 'Subtitle', 'beonepage' ),
	'section'   => 'site_front_page_blog_module',
	'default'   => 'Subtitle for blog module',
	'priority'  => 30,
	'transport' => 'postMessage',
	'js_vars'   => array(
		array(
			'element'  => '.blog-module .module-caption p',
			'function' => 'html',
			'property' => 'text'
		)
    )
) );

Kirki::add_field( 'beonepage_kirki', array(
	'type'      => 'text',
	'settings'  => 'front_page_blog_module_read_more',
	'label'     => __( 'Read More Button Text', 'beonepage' ),
	'section'   => 'site_front_page_blog_module',
	'default'   => 'Read More',
	'priority'  => 50,
	'transport' => 'postMessage',
	'js_vars'   => array(
		array(
			'element'  => '.blog-module .read-more',
			'function' => 'html',
			'property' => 'text'
		)
    )
) );

Kirki::add_field( 'beonepage_kirki', array(
	'type'      => 'text',
	'settings'  => 'front_page_blog_module_view_more',
	'label'     => __( 'View More Button Text', 'beonepage' ),
	'section'   => 'site_front_page_blog_module',
	'default'   => 'View More',
	'priority'  => 50,
	'transport' => 'postMessage',
	'js_vars'   => array(
		array(
			'element'  => '.blog-module .see-more-wrap .sm-text',
			'function' => 'html',
			'property' => 'text'
		)
    )
) );
/* Blog Module Section End */

/* Contact Module Section Start */
Kirki::add_field( 'beonepage_kirki', array(
	'type'        => 'text',
	'settings'    => 'front_page_contact_module_id',
	'label'       => __( 'Module ID', 'beonepage' ),
	'description' => __( 'Set up a unique ID for the module, the ID will be used for page scrolling navigation.', 'beonepage' ),
	'section'     => 'site_front_page_contact_module',
	'default'     => 'contact-module',
	'priority'    => 10,
	'transport'   => 'postMessage'
) );

Kirki::add_field( 'beonepage_kirki', array(
	'type'      => 'text',
	'settings'  => 'front_page_contact_module_title',
	'label'     => __( 'Title', 'beonepage' ),
	'help'      => __( 'If you want to color the word, just wrap it with \"span\" tag.', 'beonepage' ),
	'section'   => 'site_front_page_contact_module',
	'default'   => '<span>Contact</span> Module',
	'priority'  => 20,
	'transport' => 'postMessage',
	'js_vars'   => array(
		array(
			'element'  => '.contact-module .module-caption h2',
			'function' => 'html',
			'property' => 'text'
		)
    )
) );

Kirki::add_field( 'beonepage_kirki', array(
	'type'      => 'textarea',
	'settings'  => 'front_page_contact_module_subtitle',
	'label'     => __( 'Subtitle', 'beonepage' ),
	'section'   => 'site_front_page_contact_module',
	'default'   => 'Subtitle for contact module',
	'priority'  => 30,
	'transport' => 'postMessage',
	'js_vars'   => array(
		array(
			'element'  => '.contact-module .module-caption p',
			'function' => 'html',
			'property' => 'text'
		)
    )
) );

Kirki::add_field( 'beonepage_kirki', array(
	'type'      => 'text',
	'settings'  => 'front_page_contact_module_send',
	'label'     => __( 'Send Button Text', 'beonepage' ),
	'section'   => 'site_front_page_contact_module',
	'default'   => 'Send',
	'priority'  => 40,
	'transport' => 'postMessage',
	'js_vars'   => array(
		array(
			'element'  => '.contact-module button',
			'function' => 'html',
			'property' => 'text'
		)
    )
) );
/* Contact Module Section End */
/* Front Page Panel End */

Kirki::add_field( 'beonepage_kirki', array(
	'type'     => 'background',
	'setting'  => 'blog_page_header_bg',
	'label'    => __( 'Header Image', 'beonepage' ),
	'section'  => 'site_blog_page',
	'default'  => array(
		'image'    => get_template_directory_uri() . '/images/header_bg.jpg',
		'position' => 'center-center'
	),
	'priority' => 10,
	'output'   => '.page-header',
) );

/* Footer Section Start */
Kirki::add_field( 'beonepage_kirki', array(
	'type'     => 'textarea',
	'settings' => 'footer_copyright',
	'label'    => __( 'Copyright Information', 'beonepage' ),
	'help'     => __( 'Want to remove the theme byline? See section "Upgrade" for more information.', 'beonepage' ),
	'section'  => 'site_footer',
	'default'  => __( 'Copyrights &copy; 2015. All Rights Reserved.', 'beonepage'),
	'priority' => 10
) );

Kirki::add_field( 'beonepage_kirki', array(
	'type'     => 'checkbox',
	'settings' => 'footer_site_title',
	'label'    => __( 'Show Site Title?', 'beonepage' ),
	'section'  => 'site_footer',
	'default'  => '1',
	'priority' => 20
) );
/* Footer Section End */

Kirki::add_field( 'beonepage_kirki', array(
    'type'     => 'custom',
    'settings' => 'upgrade_theme_info',
    'section'  => 'upgrade_theme',
    'default'  => '<p>' . __( "Want to remove the theme byline from your website's footer?", 'beonepage' ) . '</p><p><i><a href="' . esc_url( 'http://betheme.me/themes/beonepage/' ) . '" target="_blank">' . __( 'Upgrade to BeOnePage Pro Now', 'beonepage' ) . '</a></i></p>',
    'priority' => 10
) );

/**
 * Check if it's a blog page.
 */
function beonepage_is_blog_page() {
	if ( is_front_page() ) {
		return false;
	} else {
		return true;
	}
}

/**
 * Configuration for the Kirki Customizer.
 */
function beonepage_kirki_configuration() {
	$args = array(
		'logo_image' => 'http://betheme.me/wp-content/uploads/2015/09/logo.png'
	);

	return $args;
}
add_filter( 'kirki/config', 'beonepage_kirki_configuration' );

/**
 * Change the URL that will be used by Kirki
 * to load its assets in the customizer.
 */
function beonepage_kirki_update_url( $config ) {
    $config['url_path'] = get_template_directory_uri() . '/inc/kirki/';

    return $config;
}
add_filter( 'kirki/config', 'beonepage_kirki_update_url' );
