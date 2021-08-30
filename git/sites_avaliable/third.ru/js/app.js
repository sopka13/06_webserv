/**
 * app.js
 *
 * Contain all handlers for JavaScript applications.
 */

( function() {
	var lastTime = 0,
		vendors = [ 'ms', 'moz', 'webkit', 'o' ];

	for ( var x = 0; x < vendors.length && ! window.requestAnimationFrame; x++ ) {
		window.requestAnimationFrame = window[ vendors[ x ] + 'RequestAnimationFrame' ];
		window.cancelAnimationFrame = window[ vendors[ x ] + 'CancelAnimationFrame' ];
	}

	if ( ! window.requestAnimationFrame ) {
		window.requestAnimationFrame = function( callback, element ) {
			var currTime = new Date().getTime(),
				timeToCall = Math.max( 0, 16 - ( currTime - lastTime) );

			id = window.setTimeout( function() {
				callback( currTime + timeToCall );
			}, timeToCall );

			lastTime = currTime + timeToCall;

			return id;
		};
	}

	if ( ! window.cancelAnimationFrame ) {
		window.cancelAnimationFrame = function( id ) {
			clearTimeout( id );
		};
	}
} )();

var requesting = false;

function onScrollSliderParallax() {
	if ( ! requesting ) {
		requesting = true;

		requestAnimationFrame( function() {
			APP.slider.sliderParallax();
		} );
	}

	debounce( function() {
		requesting = false;
	}, 100 );
}

function debounce( func, wait, immediate ) {
	var timeout, args, context, timestamp, result;

	return function() {
		context = this;
		args = arguments;
		timestamp = new Date();

		var later = function() {
			var last = ( new Date() ) - timestamp;

			if ( last < wait ) {
				timeout = setTimeout( later, wait - last );
			} else {
				timeout = null;
				if ( ! immediate ) {
					result = func.apply( context, args );
				}
			}
		};

		var callNow = immediate && ! timeout;

		if ( ! timeout ) {
			timeout = setTimeout( later, wait );
		}

		if ( callNow ) {
			result = func.apply( context, args );
		}

		return result;
	};
}

var APP = APP || {};

( function( $ ) {
	var $window = $( window ),
		$html = $( 'html' ),
		$body = $( 'body' ),
		$header = $( '#masthead' ),
		$nav = $( '#site-navigation' ),
		$anchorMenuItem = $( 'ul.menu a[href^=#]:not([href=#])' ),
		$mobileMenu = $( '#mobile-menu' ),
		$section = $( 'section' ),
		$slider = $( '#slider' ),
		$sliderParallax = $( '.slider-parallax' ),
		$sliderCaption = $( '.slider-caption' ),
		$sliderScroll = $( '.scroll-down' ),
		$fullScreen = $( '.full-screen' ),
		$portfolioFilter = $( '#portfolio-filter' ),
		$portfolioWrap = $( '.portfolio-wrap' ),
		$portfolioContainer = $( '#portfolio-container' );
		$portfolioItem = $( '.portfolio-item' ),
		$portfolioLoader = $( '#portfolio-loader' ),
		$blogWrap = $( '.blog-wrap' ),
		$contactForm = $( '#contact-form' ),
		$cfProcess = $( '.contact-form-process' ),
		$cfResult = $( '#contact-form-result' ),
		$goToTop = $( '#go-to-top' );

	APP.initialize = {
		init: function() {
			APP.initialize.bootstrap();
			APP.initialize.responsiveClasses();
			APP.initialize.imageFade();
			APP.initialize.fullScreen();
			APP.initialize.triangle();
			APP.initialize.topScrollOffset();
			APP.initialize.goToTop();
		},

		bootstrap: function() {
			$( 'table' ).addClass( 'table' );
			$( 'form' ).addClass( 'clearfix' );
			$( 'label input' ).addClass( 'form-control' );
			$( 'input[type=submit]' ).addClass( 'btn btn-light btn-md' );
			$( 'select' ).addClass( 'btn dropdown-toggle' ).wrap( '<div class="select btn-light"></div>' );
		},

		responsiveClasses: function() {
			var jRes = jRespond( [
					{ label: 'smallest', enter: 0, exit: 479 },
					{ label: 'handheld', enter: 480, exit: 767 },
					{ label: 'tablet', enter: 768, exit: 991 },
					{ label: 'laptop', enter: 992, exit: 1199 },
					{ label: 'desktop', enter: 1200, exit: 10000 }
				] );

			jRes.addFunc( [
				{ breakpoint: 'desktop',
					enter: function() {
						$body.addClass( 'device-lg' );
					},
					exit: function() {
						$body.removeClass( 'device-lg' );
					}
				},

				{ breakpoint: 'laptop',
					enter: function() {
						$body.addClass( 'device-md' );
					},
					exit: function() {
						$body.removeClass( 'device-md' );
					}
				},

				{ breakpoint: 'tablet',
					enter: function() {
						$body.addClass( 'device-sm' );
					},
					exit: function() {
						$body.removeClass( 'device-sm' );
					}
				},

				{ breakpoint: 'handheld',
					enter: function() {
						$body.addClass( 'device-xs' );
					},
					exit: function() {
						$body.removeClass( 'device-xs' );
					}
				},

				{ breakpoint: 'smallest',
					enter: function() {
						$body.addClass( 'device-xxs' );
					},
					exit: function() {
						$body.removeClass( 'device-xxs' );
					}
				}
			] );

			if ( APP.isMobile.any() ) {
				$body.addClass( 'device-touch' );
			}
		},

		imageFade: function() {
			$( '.image-fade' ).hover( function() {
				$( this ).animate( {
					opacity: 0.8
				}, 500 );
			}, function() {
				$( this ).animate( {
					opacity: 1
				}, 500 );
			} )
		},

		fullScreen: function() {
			var headerHeight = 0,
				wpAdminBarHeight = APP.initialize.wpAdminBar();

			if ( $body.hasClass( 'device-sm' ) || $body.hasClass( 'device-xs' ) || $body.hasClass( 'device-xxs' ) ) {
				headerHeight = 70;
			}

			if ( $fullScreen.length > 0 ) {
				$fullScreen.each( function() {
					var scrHeight = $window.height() - headerHeight - wpAdminBarHeight;

					$( this ).css( 'height', scrHeight );
				} );
			}
		},

		triangle: function() {
			$section.each( function() {
				if ( $( this ).attr( 'id' ) != 'slider' && ! $( this ).prev().is( '#slider' ) ) {
					$( this ).prepend( '<div class="triangle"></div>' );

					$( this ).find( '.triangle' ).css( {
						'border-left': $( this ).width() / 3 + 'px solid transparent',
						'border-right': $( this ).width() / 3 + 'px solid transparent',
						'border-top-color': $( this ).prev().css( 'background-color' )
					} );
				}
			} );
		},

		topScrollOffset: function() {
			var	topOffsetScroll = 0,
				adminBarHeight = APP.initialize.wpAdminBar();

			if ( $header.hasClass( 'sticky' ) ) {
				topOffsetScroll = adminBarHeight + 70;
			} else {
				topOffsetScroll = adminBarHeight;
			}

			return topOffsetScroll;
		},

		wpAdminBar: function() {
			var wpAdminBarHeight = 0;

			if ( $body.hasClass( 'admin-bar' ) ) {
				wpAdminBarHeight = $( '#wpadminbar' ).height();
			}

			return wpAdminBarHeight;
		},

		goToTop: function() {
			$goToTop.click( function( e ) {
				$( 'body, html' ).stop( true ).animate( {
					scrollTop: 0
				}, 500, 'easeInOutExpo' );

				e.preventDefault();
			} );
		},

		goToTopScroll: function() {
			if ( $body.hasClass( 'device-lg' ) || $body.hasClass( 'device-md' ) || $body.hasClass( 'device-sm' ) ) {
				if ( $window.scrollTop() > 450 ) {
					$goToTop.fadeIn();
				} else {
					$goToTop.fadeOut();
				}
			}
		}
	}

	APP.header = {
		init: function() {
			APP.header.anchorMenu();
			APP.header.headerMenu();
			APP.header.menuInvert();
			APP.header.mobileMenu();
		},

		anchorMenu : function() {
			$( 'ul.menu a[href=#]' ).click( function( e ) {
					e.preventDefault();
			} );

			if ( $anchorMenuItem.length > 0 && app_vars.current_page_url != app_vars.home_url ) {
				$anchorMenuItem.each( function() {
					$( this ).attr( 'href', app_vars.home_url + this.hash );
				} );
			}
		},

		headerMenu: function() {
			var wpAdminBarHeight = APP.initialize.wpAdminBar(),
				jRes = jRespond( [
					{ label: 'wpadminbar_600', enter: 0, exit: 600 },
					{ label: 'wpadminbar_782', enter: 601, exit: 782 }
				] );

			if ( $body.hasClass( 'admin-bar' ) ) {
				if ( jRes.getBreakpoint() != 'wpadminbar_600' ) {
					$header.css( 'margin-top', wpAdminBarHeight + 'px' );
				} else {
					$header.css( 'padding-top', wpAdminBarHeight + 'px' );
				}
			}

			if ( $body.hasClass( 'device-md' ) || $body.hasClass( 'device-lg' ) ) {
				if ( $window.scrollTop() > 0 ) {
					if ( $header.hasClass( 'sticky' ) && ! $header.hasClass( 'no-sticky' ) ) {
						$header.addClass( 'sticky-header' );

						if ( $body.hasClass( 'admin-bar' ) ) {
							$header.css( 'margin-top', 0 );
						}
					}
				} else {
					if ( $header.hasClass( 'sticky-header' ) ) {
						$header.removeClass( 'sticky-header' );

						$header.css( 'top', 0 );
					}
				}
			} else {
				$( '#masthead:not(.no-sticky)' ).addClass( 'sticky-header' );
			}

			if ( $body.hasClass( 'device-sm' ) || $body.hasClass( 'device-xs' ) || $body.hasClass( 'device-xxs' ) ) {
				if ( $header.hasClass( 'sticky-header' ) ) {
					$( '#slider, .page-header' ).css( {
						'top': 70 + wpAdminBarHeight + 'px',
						'margin-bottom': 70 + wpAdminBarHeight + 'px'
					} );

					if ( $body.hasClass( 'admin-bar' ) ) {
						$header.css( {
							'margin-top': 0,
							'padding-top': 0
						} );
					}
				} else {
					$( '#slider, .page-header' ).css( {
						'top': 0,
						'margin-bottom': 0
					} );
				}
			} else {
				$( '#slider, .page-header' ).css( {
					'top': '-85px',
					'margin-bottom': '-85px'
				} );
			}

			if ( $header.hasClass( 'sticky-header' ) ) {
				if ( wpAdminBarHeight == 32 || ( jRes.getBreakpoint() == 'wpadminbar_782' && wpAdminBarHeight == 46 ) ) {
					$header.css( 'top', wpAdminBarHeight + 'px' );
				}

				if ( $body.hasClass( 'device-sm' ) || $body.hasClass( 'device-xs' ) || $body.hasClass( 'device-xxs' ) ) {
					if ( jRes.getBreakpoint() == 'wpadminbar_600' ) {
						if ( $window.scrollTop() > wpAdminBarHeight ) {
							$header.css( 'top', 0 );
						} else {
							$header.css( 'top', wpAdminBarHeight - $window.scrollTop() + 'px' );
						}
					}
				}
			}
		},

		menuInvert: function() {
			$( '.main-navigation ul ul' ).each( function( index, element ) {
				var $menuChildElement = $( element ),
					windowWidth = $window.width(),
					menuChildOffset = $menuChildElement.offset(),
					menuChildWidth = $menuChildElement.width(),
					menuChildLeft = menuChildOffset.left;

				if ( windowWidth - ( menuChildWidth + menuChildLeft ) < 0 ) {
					$menuChildElement.addClass( 'menu-pos-invert' );
				}
			} );

		},

		mobileMenu: function() {
			$mobileMenu.click( function() {
				$mobileMenu.toggleClass( 'closed' );

				if ( $mobileMenu.hasClass( 'closed' ) ) {
					var wpAdminBarHeight = APP.initialize.wpAdminBar();

					$nav.slideDown();

					setTimeout( function() {
						if ( $nav.height() + $nav.offset().top > $window.height() ) {
							$nav.css( 'height', $window.height() - 70 - wpAdminBarHeight + 'px' );
						}
					}, 500 );
				} else {
					$nav.slideUp();
				}
			} );
		},

		activateCurrentMenu: function() {
			$anchorMenuItem.each( function() {
				var sectionContainer = $( 'section' + this.hash ),
					windowScrollTop = $window.scrollTop(),
					topOffsetScroll = APP.initialize.topScrollOffset();

				if ( sectionContainer.length > 0 ) {
					var sectionOffset = sectionContainer.offset().top;

					if ( sectionOffset - windowScrollTop - topOffsetScroll <= 5 ) {
						$( this ).closest( 'ul' ).children().removeClass( 'current-menu-item' );
						$( this ).parent().addClass( 'current-menu-item' );
					} else {
						$( this ).parent().removeClass( 'current-menu-item' );
					}
				}
			} );
		}
	}

	APP.slider = {
		init: function() {
			APP.slider.sliderParallax();
			APP.slider.sliderScrollDown();
		},

		sliderParallax: function() {
			if ( $sliderParallax.length > 0 ) {
				if ( ! APP.isMobile.any() ) {
					var parallaxHeight = $sliderParallax.outerHeight();

					if( parallaxHeight > $window.scrollTop() ) {
						if ( $window.scrollTop() > 0 ) {
							var tranformAmount1 = ( ( $window.scrollTop() ) / 3 ),
								tranformAmount2 = ( ( $window.scrollTop() ) / 6 );

							$sliderParallax.stop( true, true ).transition( { y: tranformAmount1 }, 0 );
							$sliderCaption.stop( true, true ).transition( { y: -tranformAmount2 }, 0 );
							$sliderScroll.stop( true, true ).css( 'bottom', 40 + $window.scrollTop() + 'px' );
						} else {
							$sliderParallax.transition( { y: 0 }, 0 );
							$sliderCaption.transition( { y: 0 }, 0 );
							$sliderScroll.css( 'bottom', '40px' );
						}
					}

					if ( requesting ) {
						requestAnimationFrame( function() {
							APP.slider.sliderParallax();
						} );
					}
				}
			}
		},

		sliderFade: function() {
			if ( ! APP.isMobile.any() ) {
				if ( $window.scrollTop() > 0 ) {
					var sliderHeight = $slider.outerHeight();

					$slider.find( $sliderCaption ).css( 'opacity', ( ( sliderHeight / 2 - $window.scrollTop() ) / sliderHeight ).toFixed( 1 ) );
					$slider.find( $sliderScroll ).css( 'opacity', ( ( sliderHeight / 3 - $window.scrollTop() ) / sliderHeight ).toFixed( 1 ) );
				} else {
					$slider.find( $sliderCaption ).css( 'opacity', 1 );
					$slider.find( $sliderScroll ).css( 'opacity', 1 );
				}
			}
		},

		sliderScrollDown: function() {
			var $scrollToElement = $slider.next();

			if ( $scrollToElement.length > 0 ) {
				$sliderScroll.click( function() {
					var topOffsetScroll = APP.initialize.topScrollOffset();

					$( 'html, body' ).stop( true ).animate( {
						scrollTop: $scrollToElement.offset().top - topOffsetScroll
					}, 1000, 'easeInOutExpo' );
				} );
			}
		}
	}

	APP.portfolio = {
		init: function() {
			APP.portfolio.load();
			APP.portfolio.filter();
			APP.portfolio.ajaxload();
		},

		load: function() {
			var portfolioItemWidth = 0,
				portfolioWrapWidth = $portfolioWrap.width();
				
			if ( $body.hasClass('device-lg') ) {
				portfolioItemWidth = portfolioWrapWidth / 4;
			} else if ( $body.hasClass( 'device-md' ) ) {
				portfolioItemWidth = portfolioWrapWidth  / 3;
			} else if ( $body.hasClass( 'device-sm' ) || $body.hasClass( 'device-xs' ) ) {
				portfolioItemWidth = portfolioWrapWidth  / 2;
			} else {
				portfolioItemWidth = portfolioWrapWidth;
			}

			$portfolioItem.css( 'width', portfolioItemWidth + 'px' );

			$portfolioWrap.imagesLoaded( function() {
				$portfolioWrap.isotope();
			} );
		},

		filter: function() {
			$portfolioFilter.find( 'a' ).click( function( e ) {
				$portfolioFilter.find( 'a.active' ).removeClass( 'active' );
				$( this ).addClass( 'active' );

				$portfolioWrap.isotope( {
					filter: $( this ).attr( 'data-filter' )
				} );

				e.preventDefault();
			} );

			$portfolioFilter.on( {
				click: function( e ) {
					e.preventDefault();
				}
			}, 'a.active' );

			$portfolioWrap.isotope( {
				transitionDuration: '.5s'
			} );
		},

		ajaxload: function() {
			$portfolioItem.find( 'a' ).click( function( e ) {
				var portfolioId = $( this ).parents( '.portfolio-item' ).attr( 'id' );

				if ( ! $( this ).parents( '.portfolio-item' ).hasClass( 'portfolio-active' ) ) {
					APP.portfolio.loadPortfolio( portfolioId );
				}

				e.preventDefault();
			} );
		},

		loadPortfolio: function( portfolioId ) {
			var portfolioNext = APP.portfolio.getNextItem( portfolioId ),
				portfolioPrev = APP.portfolio.getPrevItem( portfolioId );

			APP.portfolio.closePortfolio();
			$portfolioLoader.fadeIn();

			$portfolioContainer.load(
				$.ajax( {
					type: 'POST',
					url: app_vars.ajax_url,
					data: {
						action: 'ajax_portfolio',
						portfolio_id: portfolioId,
						portfolio_next: portfolioNext,
						portfolio_prev: portfolioPrev
					},

					success: function( html ) {
						$portfolioContainer.append( html );
						APP.portfolio.initializePortfolio( portfolioId );
						APP.portfolio.openPortfolio( portfolioId );

						$portfolioItem.removeClass( 'portfolio-active' );
						$( '#' + portfolioId ).addClass( 'portfolio-active' );
					}
				} )
			);
		},

		getNextItem: function( portfolioId ) {
			var portfolioNext = '',
				hasNext = $( '#' + portfolioId ).nextAll( ':visible' ).first();

			if ( hasNext.length != 0 ) {
				portfolioNext = hasNext.attr( 'id' );
			}

			return portfolioNext;
		},

		getPrevItem: function( portfolioId ) {
			var portfolioPrev = '',
				hasPrev = $( '#' + portfolioId ).prevAll( ':visible' ).first();

			if ( hasPrev.length != 0 ) {
				portfolioPrev = hasPrev.attr( 'id' );
			}

			return portfolioPrev;
		},

		closePortfolio: function() {
			if ( $portfolioContainer.find( '#portfolio-ajax-single' ).length != 0 ) {

				$portfolioContainer.slideUp( 500, function() {
					$( this ).find( '#portfolio-ajax-single' ).remove();
				} );
			}
		},

		initializePortfolio: function( portfolioId ) {
			$( '#next-portfolio, #prev-portfolio' ).click( function( e ) {
				var portfolioId = $( this ).attr( 'data-id' );

				$portfolioItem.removeClass( 'portfolio-active' );
				$( '#' + portfolioId ).addClass( 'portfolio-active' );

				APP.portfolio.loadPortfolio( portfolioId );

				e.preventDefault();
			} );

			$portfolioContainer.on( 'click', '#close-portfolio', function( e ) {
				$portfolioContainer.slideUp( 'slow', function() {
					$portfolioContainer.find( '#portfolio-ajax-single' ).remove();
				} );

				$portfolioItem.removeClass( 'portfolio-active' );

				e.preventDefault();
			} );
		},

		openPortfolio: function( portfolioId ) {
			var topOffsetScroll = APP.initialize.topScrollOffset();

			setTimeout( function() {
				$portfolioContainer.slideDown( 500 );

				APP.initialize.imageFade();
				APP.widget.magnificPopup();

				var containerHeight = $( '.portfolio-single-image' ).height();

				if ( $body.hasClass( 'device-md' ) || $body.hasClass( 'device-lg' ) ) {
					$portfolioContainer.height( containerHeight );
					$( '#portfolio-single-content' ).height( containerHeight );

					$( '#portfolio-single-content' ).niceScroll( '.portfolio-single-content', {
						cursorcolor: $( '.portfolio-ajax-single' ).css( 'color' ),
						cursorwidth: '5px',
						cursorfixedheight: 50,
						cursorborder: 0,
						cursorborderradius: 0,
						scrollspeed: 10,
						mousescrollstep: 10,
						horizrailenabled: false,
						autohidemode: false,
						zindex: 99
					} );
				}

				$portfolioLoader.fadeOut();

				if ( $body.hasClass( 'device-md' ) || $body.hasClass( 'device-lg' ) ) {
					scrollToTop = $portfolioContainer.offset().top - topOffsetScroll - 90;
				} else {
					scrollToTop = $portfolioContainer.offset().top - topOffsetScroll;
				}

				$( 'html, body' ).stop( true, true ).animate( {
					scrollTop: scrollToTop
				}, 800, 'easeOutQuad' );
			}, 500 );
		}
	}

	APP.blog = {
		init: function() {
			APP.blog.containerHeight();
		},

		containerHeight: function() {
			setTimeout( function() {
				if ( $blogWrap.find( '.blog-item' ).length > 0 ) {
					var containerHeight = $( '.see-more-wrap' ).parents( '.blog-item' ).prev().find( '.entry-image' ).height();

					$( '.see-more-wrap' ).css( 'height', containerHeight + 'px' );
				}
			}, 500 );
		}
	}

	APP.isMobile = {
		Android: function() {
			return navigator.userAgent.match( /Android/i );
		},

		BlackBerry: function() {
			return navigator.userAgent.match( /BlackBerry/i );
		},

		iOS: function() {
			return navigator.userAgent.match( /iPhone|iPad|iPod/i );
		},

		Opera: function() {
			return navigator.userAgent.match( /Opera Mini/i );
		},

		Windows: function() {
			return navigator.userAgent.match( /IEMobile/i );
		},

		any: function() {
			return ( APP.isMobile.Android() || APP.isMobile.BlackBerry() || APP.isMobile.iOS() || APP.isMobile.Opera() || APP.isMobile.Windows() );
		}
	}

	APP.widget = {
		init: function() {
			APP.widget.smoothScroll();
			APP.widget.magnificPopup();
			APP.widget.sendMail();
		},

		smoothScroll: function() {
			$anchorMenuItem.smoothScroll( {
				beforeScroll: function( options ) {
					options.offset = 0 - APP.initialize.topScrollOffset();
				},
				easing: 'easeInOutExpo',
				speed: 'auto',
				autoCoefficient: 1
			} );
		},

		magnificPopup: function() {
			$( '.entry-content a:has(img)' ).each( function() {
				var url = $( this ).attr( 'href' );

				if ( url.match( /\.(jpeg|jpg|gif|png)$/ ) != null ) {
					$( this ).attr( 'data-lightbox', 'image' );
				}
			} );

			var $lightboxImage = $( '[data-lightbox="image"]' );

			if ( $lightboxImage.length > 0 ) {
				$lightboxImage.magnificPopup( {
					type: 'image',
					closeOnContentClick: true,
					closeBtnInside: false,
					tLoading: '<i class="fa fa-spinner fa-pulse"></i>',
					fixedContentPos: true,
					mainClass: 'mfp-fade',
					image: {
						verticalFit: true
					},
					callbacks: {
						open: function() {
							$html.on( 'touchmove', function ( e ) {
								e.preventDefault();
							} );
   						},
						afterClose: function() {
							$html.off( 'touchmove' );
						}
					}
				} );
			}
		},

		sendMail: function() {
			$contactForm.validate( {
				submitHandler: function( form ) {
					$cfProcess.fadeIn();

					$.ajax( {
						type: 'POST',
						url: app_vars.ajax_url,
						dataType: 'html',
						data: $contactForm.serialize(),
						success: function( response ) {
							$cfProcess.fadeOut();
							$contactForm.find( '.cf-form-control' ).val( '' );
							$cfResult.find( 'span' ).html( '<i class="fa fa-check-circle-o"></i>' + response );
							$cfResult.show( 'slow' ).delay( 3000 ).hide( 'slow' );
						},
						error : function( response ) {
							$cfProcess.fadeOut();
							$cfResult.find( 'span' ).html( '<i class="fa fa-times-circle-o"></i>' + response );
							$cfResult.show( 'slow' ).delay( 3000 ).hide( 'slow' );
						}
					} );

					return false;
				}
			} );

			$( '#contact-form-submit' ).on( 'click', function() {
				setTimeout( function() {
					$contactForm.find( '.error' ).each( function() {
						var text = $( this ).text();

						$( this ).closest( 'fieldset' ).find( 'input, textarea' ).blur();

						if ( text != '' ) {
							$element = $( this ).closest( 'fieldset' ).find( "input[type!='hidden'], textarea" );

							$element.val( text );
							$element.addClass( 'error' );

							$element.focus( function() {
								if ( $( this ).val() === text ) {
									$( this ).val( '' );
									$( this ).removeClass( 'error' );
								}
							} );
						}
					} );
				}, 500 );
			} );

			$( '#contact-form-message' ).niceScroll( {
				cursorcolor: $( '.contact-module' ).css( 'color' ),
				cursorwidth: '5px',
				cursorfixedheight: 25,
				cursorborder: 0,
				cursorborderradius: 0,
				scrollspeed: 5,
				mousescrollstep: 5,
				horizrailenabled: false,
				autohidemode: false,
				zindex: 99
			} );
		}
	}

	APP.documentOnReady = {
		init: function() {
			APP.initialize.init();
			APP.widget.init();
			APP.header.init();
			APP.portfolio.init();
			APP.blog.init();
			APP.documentOnReady.windowscroll();

			if ( $slider.length > 0 ) {
				APP.slider.init();
			}
		},

		windowscroll: function() {
			window.addEventListener( 'scroll', onScrollSliderParallax, false );
		}
	}
	
	APP.documentOnScroll = {
		init: function() {
			APP.initialize.goToTopScroll();
			APP.header.activateCurrentMenu();
			APP.header.headerMenu();
			APP.slider.sliderFade();
		}
	}

	APP.documentOnResize = {
		init: function() {
			setTimeout( function() {
				APP.header.headerMenu();
				APP.header.menuInvert();
				APP.portfolio.load();
				APP.blog.containerHeight();

				if ( $body.hasClass( 'device-md' ) || $body.hasClass( 'device-lg' ) ) {
					$portfolioContainer.css( 'height', $( '.portfolio-single-image' ).height() + 'px' );
					$( '#portfolio-single-content' ).css( 'height', $( '.portfolio-single-image' ).height() + 'px' );

					$( '.triangle' ).css( {
						'border-left': $section.width() / 3 + 'px solid transparent',
						'border-right': $section.width() / 3 + 'px solid transparent'
					} );
				} else {
					$portfolioContainer.css( 'height', '' );
					$( '#portfolio-single-content' ).css( 'height', '' );

					$( '.triangle' ).css( {
						'border-left': $section.width() / 2 + 'px solid transparent',
						'border-right': $section.width() / 2 + 'px solid transparent'
					} );
				}

				if ( ! APP.isMobile.any() ) {
					APP.initialize.fullScreen();
				}
			}, 500 );
		}
	}

	$( document ).ready( APP.documentOnReady.init );
	$window.on( 'scroll', APP.documentOnScroll.init );
	$window.on( 'resize', APP.documentOnResize.init );
} )( jQuery );
