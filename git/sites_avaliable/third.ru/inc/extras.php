<?php
/**
 * Custom functions that act independently of the theme templates.
 *
 * Eventually, some of the functionality here could be replaced by core features.
 *
 * @package BeOnePage
 */

/**
 * Adds custom classes to the array of body classes.
 *
 * @param array $classes Classes for the body element.
 * @return array
 */
function beonepage_body_classes( $classes ) {
	// Adds a class of group-blog to blogs with more than 1 published author.
	if ( is_multi_author() ) {
		$classes[] = 'group-blog';
	}

	// Adds a class for front page.
	if ( is_front_page() ) {
		$classes[] = 'front-page';
	}

	return $classes;
}
add_filter( 'body_class', 'beonepage_body_classes' );

/**
 * Get theme version.
 *
 * @return string $theme_version The theme version.
 */
function beonepage_get_version() {
	$theme_info = wp_get_theme();

	// If it's a child theme, then get parent theme.
	if ( is_child_theme() ) {
		$theme_info = wp_get_theme( $theme_info->parent_theme );
	}

	$theme_version = $theme_info->display( 'Version' );

	return $theme_version;
}

/**
 * Register the required plugins for this theme.
 *
 * @link https://github.com/TGMPA/TGM-Plugin-Activation
 */
function beonepage_register_required_plugins() {
	// Required plugin.
	$plugins = array(
		array(
			'name'               => 'BeOnePage Lite Plugin',
			'slug'               => 'beonepage-lite-plugin',
			'source'             => get_template_directory_uri() . '/plugins/beonepage-lite-plugin.zip',
			'required'           => true,
			'version'            => '1.0.0',
			'force_activation'   => true,
			'force_deactivation' => true
		),
	);

	// Array of configuration settings.
	$config = array(
		'id'           => 'beonepage_tgmpa',
		'default_path' => '',
		'menu'         => 'beonepage-install-plugins',
		'parent_slug'  => 'themes.php',
		'capability'   => 'edit_theme_options',
		'has_notices'  => true,
		'dismissable'  => true,
		'is_automatic' => true
	);

	tgmpa( $plugins, $config );
}
add_action( 'tgmpa_register', 'beonepage_register_required_plugins' );

/**
 * The notification to upgrade to Pro version.
 *
 * @return string $output The upgrade notification.
 */
function beonepage_premium_info() {
	$output = sprintf (
				'%1$s <a href="%2$s" target="_blank">BeOnePage Pro</a> %3$s <a href="%4$s" class="thickbox" title="BeOnePage Pro">%5$s</a> %6$s.',
				esc_html__( 'Upgrade to', 'beonepage' ),
				esc_url( 'http://betheme.me/themes/beonepage/' ),
				esc_html__( 'to enjoy', 'beonepage' ),
				esc_url( 'http://betheme.me/themes/beonepage/?TB_iframe=true&width=1024&height=800' ),
				esc_html__( 'Premium Features', 'beonepage' ),
				esc_html__( 'and support continued development', 'beonepage' )
			);

	return $output;
}

/**
 * Only show blog posts in search results.
 *
 * @param array $query The WP_Query object.
 */

function beonepage_search_filter( $query ) {
    if ( ! is_admin() && $query->is_main_query() ) {
		if ( $query->is_search ) {
			$query->set( 'post_type', array( 'post' ) );
		}
	}
}
add_filter( 'pre_get_posts', 'beonepage_search_filter' );

/**
 * Get the current URL of the page being viewed.
 *
 * @global object $wp
 * @return string $current_url Current URL.
 */
function beonepage_get_current_url() {
	global $wp;

	if ( empty( $_SERVER['QUERY_STRING'] ) )
		$current_url = trailingslashit( home_url( $wp->request ) );
	else
		$current_url = add_query_arg( $_SERVER['QUERY_STRING'], '', trailingslashit( home_url( $wp->request ) ) );

	return $current_url;
}

/**
 * Filter to remove thumbnail image dimension attributes.
 *
 * @return string $html The HTML codes without width and height attributes.
 */
function beonepage_remove_thumbnail_dimensions( $html, $post_id, $post_image_id ) {
    $html = preg_replace( '/(width|height)=\"\d*\"\s/', '', $html );

    return $html;
}
add_filter( 'post_thumbnail_html', 'beonepage_remove_thumbnail_dimensions', 10, 3 );

/**
 * Change the excerpt length.
 */
function beonepage_custom_excerpt_length( $length ) {
	return 60;
}
add_filter( 'excerpt_length', 'beonepage_custom_excerpt_length', 999 );

/**
 * Change the excerpt more string at the end.
 */
function beonepage_new_excerpt_more( $more ) {
	return ' &hellip;';
}
add_filter( 'excerpt_more', 'beonepage_new_excerpt_more' );

/**
 * Include the Portfolio details template.
 */
function beonepage_ajax_portfolio() {
	get_template_part( 'template-parts/content', 'ajax-portfolio' );

	wp_die();
}
add_action( 'wp_ajax_ajax_portfolio', 'beonepage_ajax_portfolio' );
add_action( 'wp_ajax_nopriv_ajax_portfolio', 'beonepage_ajax_portfolio' );

/**
 * Send mail using wp_mail().
 */
function beonepage_contact_send_message() {
	if ( ! wp_verify_nonce( $_POST['ajax_contact_form_nonce'], 'ajax_contact_form' ) ) {
		$response = esc_html_e( 'Verification error. Try again!', 'beonepage' );
	} else {
		$to       = get_option( 'admin_email' );
		$name     = sanitize_text_field( $_POST['name'] );
		$email    = sanitize_email( $_POST['email'] );
		$phone    = sanitize_text_field( $_POST['phone'] );
		$subject  = sanitize_text_field( $_POST['subject'] );
		$message  = sanitize_text_field( $_POST['message'] );
		$headers  = 'From: ' . $name . ' <' . $email . '>' . "\r\n";
		$headers .= "Reply-To: $email\r\n";

		if ( $phone != '' ) {
			$subject .= ', from: ' . $name . ', ' . __( 'phone', 'beonepage' ) . ': ' . $phone ;
		}

		// Send the email using wp_mail().
		if ( wp_mail( $to, $subject, $message, $headers ) ) {
			$response = esc_html_e( 'Thank you. The Mailman is on his way!', 'beonepage' );
		} else {
			$response = esc_html_e( "Sorry, don't know what happened. Try later!", 'beonepage' );
		}
	}

	wp_die();
}
add_action( 'wp_ajax_contact_form', 'beonepage_contact_send_message' );
add_action( 'wp_ajax_nopriv_contact_form', 'beonepage_contact_send_message' );

/**
 * Add numeric pagination.
 */
function beonepage_numeric_pagination( $pages = '', $range = 2 ) {
     $showitems = ( $range * 2 ) + 1;

     global $paged;

     if ( empty( $paged ) ) $paged = 1;

     if ( $pages == '' ) {
		global $wp_query;

		$pages = $wp_query->max_num_pages;

		if ( ! $pages ) {
			$pages = 1;
		}
	}

	if ( $pages != 1 ) {
		echo '<nav class="posts-navigation text-center hidden-xs clearfix" role="navigation">';
		echo '<ul>';

		if( $paged > 2 && $paged > $range + 1 && $showitems < $pages ) {
			echo '<li><a href=' . get_pagenum_link( 1 ) . '>' . esc_html__( 'First', 'beonepage' ) . '</a></li>';
		}

		if( $paged > 1 && $showitems < $pages ) {
			echo '<li><a href=' . get_pagenum_link( $paged - 1 ) . '>' . esc_html__( 'Prev', 'beonepage' ) . '</a></li>';
		}

		for ( $i = 1; $i <= $pages; $i++ ) {
			if ( 1 != $pages && ( ! ( $i >= $paged + $range + 1 || $i <= $paged - $range - 1 ) || $pages <= $showitems ) ) {
				echo ( $paged == $i ) ? '<li class="active"><a href=' . get_pagenum_link( $i ) . '>' . $i . '</a></li>' : '<li><a href=' . get_pagenum_link( $i ) . '>' . $i . '</a></li>';
			}
		}

		if ( $paged < $pages && $showitems < $pages) {
			echo '<li><a href=' . get_pagenum_link( $paged + 1 ) . '>' . esc_html__( 'Next', 'beonepage' ) . '</a></li>';
		}

		if ( $paged < $pages - 1 &&  $pages > $paged + $range - 1 && $showitems < $pages ) {
			echo '<li><a href=' . get_pagenum_link( $pages ) . '>' . esc_html__( 'Last', 'beonepage' ) . '</a></li>';
		}

		echo '</ul>';
		echo '</nav>';
	}
}

/**
 * Set/unset post as image post type if post has thumbnail.
 *
 * @param int $post_id The post ID.
 */
function beonepage_set_post_type( $post_id ) {
	global $pagenow; 

	if ( in_array( $pagenow, array( 'post.php', 'post-new.php' ) ) ) {
		if ( get_post_type( $post_id ) == 'post' ) {
			if ( has_post_thumbnail( $post_id ) ) {
				set_post_format( $post->ID, 'image' );
			}

			if ( ! has_post_thumbnail( $post_id ) ) {
				set_post_format( $post->ID, '' );
			}
		}
	}
}
add_action( 'save_post', 'beonepage_set_post_type', 10, 3 );

/**
 * Remove WordPress Admin Bar style from header.
 */
function beonepage_remove_admin_bar_style() {
	remove_action( 'wp_head', '_admin_bar_bump_cb' );
}
add_action( 'get_header', 'beonepage_remove_admin_bar_style' );

/**
 * Remove Recent Comments Widget style from header.
 */
function beonepage_remove_recent_comments_style() {  
	global $wp_widget_factory;  

	remove_action( 'wp_head', array( $wp_widget_factory->widgets['WP_Widget_Recent_Comments'], 'recent_comments_style' ) );  
}  
add_action( 'widgets_init', 'beonepage_remove_recent_comments_style' );

/**
 * Change the font size for Tag Cloud widget.
 */
function beonepage_custom_tag_cloud_font( $args ) {
	$args = array( 'smallest' => 10, 'largest' => 10 );

	return $args;
}
add_filter( 'widget_tag_cloud_args', 'beonepage_custom_tag_cloud_font' );

/**
 * Hide editor on Front Page.
 *
 */
function beonepage_hide_editor() {
	global $pagenow, $post;

	if ( ! ( $pagenow == 'post.php' ) ) {
		return;
	}

	$post_id = $_GET['post'] ? $_GET['post'] : $_POST['post_ID'];

	if ( ! isset( $post_id ) ) {
		return;
	};

	if ( $post_id == get_option( 'page_on_front' ) ) {
		remove_post_type_support( 'page', 'editor' );
	}
}
add_action( 'admin_head', 'beonepage_hide_editor' );

/**
 * Update option after Customizer save.
 */
function beonepage_customize_save_after() {
	update_option( 'blogname', Kirki::get_option( 'general_site_title' ) );

	if ( Kirki::get_option( 'general_front_page' ) != '0' ) {
		update_option( 'show_on_front ', 'page' );
		update_option( 'page_on_front', Kirki::get_option( 'general_front_page' ) );
		update_option( 'page_for_posts', Kirki::get_option( 'general_posts_page' ) );
	}
}
add_action( 'customize_save_after', 'beonepage_customize_save_after' );

/**
 * Set option data when Customizer controls are initialized.
 */
function beonepage_customize_controls_init() {
	set_theme_mod( 'general_site_title', get_bloginfo( 'name' ) );
	set_theme_mod( 'general_front_page', get_option( 'page_on_front' ) );
	set_theme_mod( 'general_posts_page', get_option( 'page_for_posts' ) );
}
add_action( 'customize_controls_init', 'beonepage_customize_controls_init' );

/**
 * Enqueue scripts and styles for admin pages.
 */
function beonepage_admin_scripts() {
	global $pagenow;

	if ( ! is_admin() ) {
		return;
	}

	if ( in_array( $pagenow, array( 'nav-menus.php', 'post.php', 'post-new.php', 'themes.php', 'update-core.php' ) ) ) {
		wp_enqueue_script( 'beonepage-admin-script', get_template_directory_uri() . '/js/admin.js', array(), beonepage_get_version(), true );
	}

	// Localize the script with new data.
	wp_localize_script( 'beonepage-admin-script', 'admin_vars', array(
		'screen'            => $pagenow,
		's_icon_found'      => esc_html__( 'icon found.', 'beonepage' ),
		'p_icons_found'     => esc_html__( 'icons found.', 'beonepage' ),
		'no_icons_found'    => esc_html__( 'No icons found.', 'beonepage' ),
		'upgrade_available' => esc_html__( 'Upgrade to Pro Version', 'beonepage' ),
		'upgrade_info'      => beonepage_premium_info()
	) );
}
add_action( 'admin_enqueue_scripts', 'beonepage_admin_scripts' );

/**
 * Get font icons list.
 *
 * @return array $font_icons
 */
function beonepage_icon_list() {
	$font_icons = array(
		'500px', 'adjust', 'adn', 'align-center', 'align-justify', 'align-left', 'align-right', 'amazon', 'ambulance', 'anchor', 'android', 'angellist', 'angle-double-down', 'angle-double-left', 'angle-double-right', 'angle-double-up', 'angle-down', 'angle-left', 'angle-right', 'angle-up', 'apple', 'archive', 'area-chart', 'arrow-circle-down', 'arrow-circle-left', 'arrow-circle-o-down', 'arrow-circle-o-left', 'arrow-circle-o-right', 'arrow-circle-o-up', 'arrow-circle-right', 'arrow-circle-up', 'arrow-down', 'arrow-left', 'arrow-right', 'arrow-up', 'arrows', 'arrows-alt', 'arrows-h', 'arrows-v', 'asterisk', 'at', 'automobile', 'backward', 'balance-scale', 'ban', 'bank', 'bar-chart', 'bar-chart-o', 'barcode', 'bars', 'battery-0', 'battery-1', 'battery-2', 'battery-3', 'battery-4', 'battery-empty', 'battery-full', 'battery-half', 'battery-quarter', 'battery-three-quarters', 'bed', 'beer', 'behance', 'behance-square', 'bell', 'bell-o', 'bell-slash', 'bell-slash-o', 'bicycle', 'binoculars', 'birthday-cake', 'bitbucket', 'bitbucket-square', 'bitcoin', 'black-tie', 'bold', 'bolt', 'bomb', 'book', 'bookmark', 'bookmark-o', 'briefcase', 'btc', 'bug', 'building', 'building-o', 'bullhorn', 'bullseye', 'bus', 'buysellads', 'cab', 'calculator', 'calendar', 'calendar-check-o', 'calendar-minus-o', 'calendar-o', 'calendar-plus-o', 'calendar-times-o', 'camera', 'camera-retro', 'car', 'caret-down', 'caret-left', 'caret-right', 'caret-square-o-down', 'caret-square-o-left', 'caret-square-o-right', 'caret-square-o-up', 'caret-up', 'cart-arrow-down', 'cart-plus', 'cc', 'cc-amex', 'cc-diners-club', 'cc-discover', 'cc-jcb', 'cc-mastercard', 'cc-paypal', 'cc-stripe', 'cc-visa', 'certificate', 'chain', 'chain-broken', 'check', 'check-circle', 'check-circle-o', 'check-square', 'check-square-o', 'chevron-circle-down', 'chevron-circle-left', 'chevron-circle-right', 'chevron-circle-up', 'chevron-down', 'chevron-left', 'chevron-right', 'chevron-up', 'child', 'chrome', 'circle', 'circle-o', 'circle-o-notch', 'circle-thin', 'clipboard', 'clock-o', 'clone', 'close', 'cloud', 'cloud-download', 'cloud-upload', 'cny', 'code', 'code-fork', 'codepen', 'coffee', 'cog', 'cogs', 'columns', 'comment', 'comment-o', 'commenting', 'commenting-o', 'comments', 'comments-o', 'compass', 'compress', 'connectdevelop', 'contao', 'copy', 'copyright', 'creative-commons', 'credit-card', 'crop', 'crosshairs', 'css3', 'cube', 'cubes', 'cut', 'cutlery', 'dashboard', 'dashcube', 'database', 'dedent', 'delicious', 'desktop', 'deviantart', 'diamond', 'digg', 'dollar', 'dot-circle-o', 'download', 'dribbble', 'dropbox', 'drupal', 'edit', 'eject', 'ellipsis-h', 'ellipsis-v', 'empire', 'envelope', 'envelope-o', 'envelope-square', 'eraser', 'eur', 'euro', 'exchange', 'exclamation', 'exclamation-circle', 'exclamation-triangle', 'expand', 'expeditedssl', 'external-link', 'external-link-square', 'eye', 'eye-slash', 'eyedropper', 'facebook', 'facebook-f', 'facebook-official', 'facebook-square', 'fast-backward', 'fast-forward', 'fax', 'feed', 'female', 'fighter-jet', 'file', 'file-archive-o', 'file-audio-o', 'file-code-o', 'file-excel-o', 'file-image-o', 'file-movie-o', 'file-o', 'file-pdf-o', 'file-photo-o', 'file-picture-o', 'file-powerpoint-o', 'file-sound-o', 'file-text', 'file-text-o', 'file-video-o', 'file-word-o', 'file-zip-o', 'files-o', 'film', 'filter', 'fire', 'fire-extinguisher', 'firefox', 'flag', 'flag-checkered', 'flag-o', 'flash', 'flask', 'flickr', 'floppy-o', 'folder', 'folder-o', 'folder-open', 'folder-open-o', 'font', 'fonticons', 'forumbee', 'forward', 'foursquare', 'frown-o', 'futbol-o', 'gamepad', 'gavel', 'gbp', 'ge', 'gear', 'gears', 'genderless', 'get-pocket', 'gg', 'gg-circle', 'gift', 'git', 'git-square', 'github', 'github-alt', 'github-square', 'gittip', 'glass', 'globe', 'google', 'google-plus', 'google-plus-square', 'google-wallet', 'graduation-cap', 'gratipay', 'group', 'h-square', 'hacker-news', 'hand-grab-o', 'hand-lizard-o', 'hand-o-down', 'hand-o-left', 'hand-o-right', 'hand-o-up', 'hand-paper-o', 'hand-peace-o', 'hand-pointer-o', 'hand-rock-o', 'hand-scissors-o', 'hand-spock-o', 'hand-stop-o', 'hdd-o', 'header', 'headphones', 'heart', 'heart-o', 'heartbeat', 'history', 'home', 'hospital-o', 'hotel', 'hourglass', 'hourglass-1', 'hourglass-2', 'hourglass-3', 'hourglass-end', 'hourglass-half', 'hourglass-o', 'hourglass-start', 'houzz', 'html5', 'i-cursor', 'ils', 'image', 'inbox', 'indent', 'industry', 'info', 'info-circle', 'inr', 'instagram', 'institution', 'internet-explorer', 'intersex', 'ioxhost', 'italic', 'joomla', 'jpy', 'jsfiddle', 'key', 'keyboard-o', 'krw', 'language', 'laptop', 'lastfm', 'lastfm-square', 'leaf', 'leanpub', 'legal', 'lemon-o', 'level-down', 'level-up', 'life-bouy', 'life-buoy', 'life-ring', 'life-saver', 'lightbulb-o', 'line-chart', 'link', 'linkedin', 'linkedin-square', 'linux', 'list', 'list-alt', 'list-ol', 'list-ul', 'location-arrow', 'lock', 'long-arrow-down', 'long-arrow-left', 'long-arrow-right', 'long-arrow-up', 'magic', 'magnet', 'mail-forward', 'mail-reply', 'mail-reply-all', 'male', 'map', 'map-marker', 'map-o', 'map-pin', 'map-signs', 'mars', 'mars-double', 'mars-stroke', 'mars-stroke-h', 'mars-stroke-v', 'maxcdn', 'meanpath', 'medium', 'medkit', 'meh-o', 'mercury', 'microphone', 'microphone-slash', 'minus', 'minus-circle', 'minus-square', 'minus-square-o', 'mobile', 'mobile-phone', 'money', 'moon-o', 'mortar-board', 'motorcycle', 'mouse-pointer', 'music', 'navicon', 'neuter', 'newspaper-o', 'object-group', 'object-ungroup', 'odnoklassniki', 'odnoklassniki-square', 'opencart', 'openid', 'opera', 'optin-monster', 'outdent', 'pagelines', 'paint-brush', 'paper-plane', 'paper-plane-o', 'paperclip', 'paragraph', 'paste', 'pause', 'paw', 'paypal', 'pencil', 'pencil-square', 'pencil-square-o', 'phone', 'phone-square', 'photo', 'picture-o', 'pie-chart', 'pied-piper', 'pied-piper-alt', 'pinterest', 'pinterest-p', 'pinterest-square', 'plane', 'play', 'play-circle', 'play-circle-o', 'plug', 'plus', 'plus-circle', 'plus-square', 'plus-square-o', 'power-off', 'print', 'puzzle-piece', 'qq', 'qrcode', 'question', 'question-circle', 'quote-left', 'quote-right', 'ra', 'random', 'rebel', 'recycle', 'reddit', 'reddit-square', 'refresh', 'registered', 'remove', 'renren', 'reorder', 'repeat', 'reply', 'reply-all', 'retweet', 'rmb', 'road', 'rocket', 'rotate-left', 'rotate-right', 'rouble', 'rss', 'rss-square', 'rub', 'ruble', 'rupee', 'safari', 'save', 'scissors', 'search', 'search-minus', 'search-plus', 'sellsy', 'send', 'send-o', 'server', 'share', 'share-alt', 'share-alt-square', 'share-square', 'share-square-o', 'shekel', 'sheqel', 'shield', 'ship', 'shirtsinbulk', 'shopping-cart', 'sign-in', 'sign-out', 'signal', 'simplybuilt', 'sitemap', 'skyatlas', 'skype', 'slack', 'sliders', 'slideshare', 'smile-o', 'soccer-ball-o', 'sort', 'sort-alpha-asc', 'sort-alpha-desc', 'sort-amount-asc', 'sort-amount-desc', 'sort-asc', 'sort-desc', 'sort-down', 'sort-numeric-asc', 'sort-numeric-desc', 'sort-up', 'soundcloud', 'space-shuttle', 'spinner', 'spoon', 'spotify', 'square', 'square-o', 'stack-exchange', 'stack-overflow', 'star', 'star-half', 'star-half-empty', 'star-half-full', 'star-half-o', 'star-o', 'steam', 'steam-square', 'step-backward', 'step-forward', 'stethoscope', 'sticky-note', 'sticky-note-o', 'stop', 'street-view', 'strikethrough', 'stumbleupon', 'stumbleupon-circle', 'subscript', 'subway', 'suitcase', 'sun-o', 'superscript', 'support', 'table', 'tablet', 'tachometer', 'tag', 'tags', 'tasks', 'taxi', 'television', 'tencent-weibo', 'terminal', 'text-height', 'text-width', 'th', 'th-large', 'th-list', 'thumb-tack', 'thumbs-down', 'thumbs-o-down', 'thumbs-o-up', 'thumbs-up', 'ticket', 'times', 'times-circle', 'times-circle-o', 'tint', 'toggle-down', 'toggle-left', 'toggle-off', 'toggle-on', 'toggle-right', 'toggle-up', 'trademark', 'train', 'transgender', 'transgender-alt', 'trash', 'trash-o', 'tree', 'trello', 'tripadvisor', 'trophy', 'truck', 'try', 'tty', 'tumblr', 'tumblr-square', 'turkish-lira', 'tv', 'twitch', 'twitter', 'twitter-square', 'umbrella', 'underline', 'undo', 'university', 'unlink', 'unlock', 'unlock-alt', 'unsorted', 'upload', 'usd', 'user', 'user-md', 'user-plus', 'user-secret', 'user-times', 'users', 'venus', 'venus-double', 'venus-mars', 'viacoin', 'video-camera', 'vimeo', 'vimeo-square', 'vine', 'vk', 'volume-down', 'volume-off', 'volume-up', 'warning', 'wechat', 'weibo', 'weixin', 'whatsapp', 'wheelchair', 'wifi', 'wikipedia-w', 'windows', 'won', 'wordpress', 'wrench', 'xing', 'xing-square', 'y-combinator', 'y-combinator-square', 'yahoo', 'yc', 'yc-square', 'yelp', 'yen', 'youtube', 'youtube-play', 'youtube-square'
	);

	return $font_icons;
}
