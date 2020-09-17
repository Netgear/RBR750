/**
 * @fileOverview Javascript for net02
 * @author werner@bussedesign.com
 * @requires jQuery 1.11.0
 */

// the jQuery wrapper to prevent conflicts with other libraries
(function ($) {
    
    "use strict";

    /*global $, jQuery, window, document, console*/
    /*jslint plusplus: true, vars: true, indent: 2, bitwise: true*/
  
  
    /**
     *  to deal with touch and clicks we use a jquery event extension >> touchclick
     *  to detect touch we use modernizr
     *
     */
    var oldIE = false,
        hasTouch = false,
        isTablet = false,
        isPhone  = false,
        isUnknown= false,       /* for pages cannot recognized by nav button */
        currentWidth = $(window).innerWidth(),     /* to record current width */
        eventType =""; 
     
     
    // detecting IE version
    if ($('html').hasClass('ie8')) {
        oldIE = true;
    }

    if ($('html').hasClass('touch')) {
      hasTouch = true;
    }
     
    eventType = hasTouch ? "touchstart" : "click";

    // compatibility for tablets with both touch and pen input
    // such as "Microsoft Surface"
    eventType = "click";
    
    jQuery.event.special.touchclick = {
      bindType: eventType,
      delegateType: eventType
    };
  
             
    /**
     *  the document ready function
     */
    $(function () {
    
    
      /**
       *  We are not checking for any device type as that is unreliable. 
       *  Rather we are defining behavior based on media query breakpoints that change
       *  the ui. 
       *  For screen sizes > 980px width the main nav is located in the left sidebar and
       *  visible.
       *  For screens < 980px wide a "hamburger" icon is added to the header and the main
       *  nav is hidden by the content. When the "hamburger" is clicked, content area 
       *  slides to the right, revealing the main nav.
       *  For screens < 600px wide the main nav will just slide down from the header
       *  when the "hamburger" is clicked.
       *  using 'isTablet' for screen width from 600 to 980px
       *  We are using two variables to indicate the states of the nav: isTablet and isPhone
       *  
       *  isTablet is true when the "hamburger" is visible (#navButton)
       *  isPhone is true when the "hamburger" is visible and the byline has float left applied
       *
       *  These conditions are set via media queries in the main.css
       *
       */
      
      if ($('#navButton').css('display') === 'block') {
        isTablet = true;
      }
      
      if (($('#navButton').css('display') === 'block') && ($('#byline').css('float') === 'left')) {
        isTablet = false;
        isPhone = true;
      }

      if (($('#navButton').css('display') === undefined) || ($('#byline').css('float') === undefined)) {
        isUnknown = true;
      }

      if (window.console && console.log)
      {
        console.log("Tablet="+isTablet);
        console.log("Phone="+isPhone);
        console.log("Unknown="+isUnknown);
      }
      
    
      // global constants
      var MIN_PWD_CHARACTERS = 8,
        MAX_PWD_CHARACTERS = 64,
        
        //system password
        MIN_USERPWD_CHARACTERS = 8,
        MAX_USERPWD_CHARACTERS = 32,
        
        //a@b.xx
        MIN_EMAIL_CHARACTERS = 6,
        //tools.ietf.org/html/rfc5321#section-4.5.3, 256 = '<' + 254 + '>'
        MAX_EMAIL_CHARACTERS = 254,
        
        // network name
        NAME_EXISTS      = 0x80,
        // password types
        SAME_PWD         = 0x40,
        DIFF_PWD         = 0x20,
        NO_PWD           = 0x10,
        // network select
        MANUAL_SELECT    = 0x40,
        AUTO_SELECT      = 0x20,
        NOT_DFS          = 0x10,
        //DISABLED         = 0x10,
        // security options
        SEC_OPTION_NONE  = 0x08,
        SEC_OPTION       = 0x04,
        // valid password
        VALID_PWD        = 0x02,
        
        DISABLED         = 0x01, //disabled must be unique, otherwise both networks with no pwd will be wrong

        // reset patterns
        RESET_SEC_OPTIONS              = 0x0e, //SEC_OPTION_NONE | SEC_OPTION | VALID_PWD
        RESET_PWD_TYPE                 = 0x70,
        RESET_SELECT                   = 0x61,
        RESET_SEC_OPTIONS_AND_PWD_TYPE = 0x6d,
        RESET_ALL                      = 0xFF,
        validStates=[];
        
        
      var web_timeout_min = parseInt('<% nvram_get("http_timeout"); %>',10), // unit:minute
        web_timeout = web_timeout_min * 60,
        web_timeout_warning = (web_timeout_min - 1) * 60,
        web_time_elapsed = 0,
        web_time_last_access = 0,
        server_is_timeout = false,
        web_time_step = 0, //0, 1: web_timeout_warning, 2: click cancel in warning, 3: web_timeout_hint
        web_time_need_extend = false;
      var web_time_init_date = new Date();
      var web_time_extend_date = new Date();
      
      function ajaxExtendAccessTime(postStr) {
        $.ajax({
           type: "POST",
           url: "ajax/extend_access_time",
           data: postStr,
           dataType: "html",
           success: function(data) {
                if (data == "success") {
                  if (web_time_need_extend) {
                    web_time_need_extend = false;
                    web_time_init_date = web_time_extend_date;
                  }
                  else {
                    web_time_init_date = new Date();
                  }
                  web_time_step = 0;
                  web_timeout_warning = (web_timeout_min - 1) * 60;
                } else {
                  server_is_timeout = true;
                }
           },
           error: function(responseText, textStatus, XMLHttpRequest) {
                if (window.console && console.log) {
                    console.log("responseText:", responseText);
                    console.log("textStatus:", textStatus);
                    console.log("XMLHttpRequest:", XMLHttpRequest);
                }
           }
        });
      }

      function accessTimer() {
        var cur_date = new Date();
        web_time_elapsed = (cur_date.getTime() - web_time_init_date.getTime())/1000;
        
        if ((web_time_step == 0) && (web_time_elapsed >= web_timeout_warning)) {
          if (web_time_need_extend) {
            web_time_elapsed = web_timeout_warning - web_time_last_access;
            var postStr = "web_time_last_access=" + web_time_elapsed;
            web_timeout_warning += 30;
            ajaxExtendAccessTime(postStr);
          } else {
            $('#screenOverlay').addClass('active');
            // check if screen is covered by modalOverlay
            if ($('#modalOverlay').hasClass('active'))
              $('#screenOverlay').addClass('light');
            
            $('#web_timeout_warning').fadeIn();
            web_time_step = 1;
          }
        }

        /* pass a function reference (can be an anonymous function) */
        var timerID = setTimeout(function() {accessTimer();}, 1000);
        cur_date = new Date();
        web_time_elapsed = (cur_date.getTime() - web_time_init_date.getTime())/1000;

        if ((web_time_elapsed > web_timeout) || server_is_timeout) {
          clearTimeout(timerID);
          $('#screenOverlay').addClass('active');
          $('#web_timeout_warning').fadeOut();
          $('#web_timeout_hint').fadeIn();
          web_time_step = 3;
        }
      }

      var web_pwd_off = $("input[name='web_pwd_off']");
      var web_blank_state = $("input[name='web_blank_state']");
      if (web_pwd_off.length && web_pwd_off.val() != 1
          && web_blank_state.length && web_blank_state.val() == 0) {

        var screenOverlay = "<div id='screenOverlay'></div>";
        var popupWarning = "", popupHint = "";
        popupWarning += "<div id='web_timeout_warning' class='dashboardPopup timeout' style='display:none'>";
        popupWarning += "<p><% getstr("timeout_warning"); %></p><br>";
        popupWarning += "<a class='btn primary close extendTime'><% getstr("yes_mark"); %></a>";
        popupWarning += "<a class='btn secondary close'><% getstr("no_mark"); %></a></div>";
        
        popupHint += "<div id='web_timeout_hint' class='dashboardPopup timeout' style='display:none'>";
        popupHint += "<p><% getstr("timeout"); %></p><br>";
        popupHint += "<a class='btn primary close'><% getstr("ok_mark"); %></a></div>";

        // prepend elements to <body>, avoid overlap covered issue
        $(popupHint).prependTo($('body'));
        $(popupWarning).prependTo($('body'));
        $(screenOverlay).prependTo($('body'));

        $(window).on("keydown click scroll", function () {
          var cur_date = new Date();
          web_time_elapsed = (cur_date.getTime() - web_time_init_date.getTime())/1000.
          if (web_time_elapsed < web_timeout_warning) {
            web_time_need_extend = true;
            web_time_last_access = web_time_elapsed;
            web_time_extend_date = new Date();
          }
        });

        $('.extendTime').click(function () {
          var postStr = "extend_time=" + web_timeout;
          ajaxExtendAccessTime(postStr);
        });

        $('#web_timeout_warning').find('a').click(function () {
          $('#web_timeout_warning').fadeOut();
          $('#screenOverlay').removeClass('active');
          web_time_step = 2;
        });

        $('#web_timeout_hint').find('a').click(function () {
          $('#web_timeout_hint').fadeOut();
          $('#screenOverlay').removeClass('active');
          web_time_step = 4;
          top.location.replace("start.htm");
        });

        accessTimer();
      }

      /**
       *  @function setStatusFlag
       *  @param  {Object} obj - the network container
       *  @param  {Number}  statusFlag -a hex number status flag that is used to update the status byte
       *  @param  {Boolean}  set - If true we set the flag, if false we reset the flag
       *  @param  {array} validStates - holds the valid states to enable the next button
       *  @return {Boolean} Returns true if status byte indicates button enable
       *
       *  This function updates the status byte of the next button enable state. After the update it
       *  tests the status byte if it matches any enabling state. It then updates the buttonEnable 
       *  in the object data and return true if the button can be enabled.
       */
      function setStatusFlag(obj, statusFlag, set) {
      
        var statusByte = obj.data('submitStatus'),
          allowNext = false,
          i;
            
        if (set) {
          // set the individual status flag
          statusByte = statusByte | statusFlag;
        } else {
          // reset the individual status flag
          statusByte = statusByte & ~statusFlag;
        }
        // update the status byte
        obj.data('submitStatus', statusByte);
        
        // we are using a global array which holds the valid states for this page
        // cycle through the enable states and see if one matches the status byte
        for (i = 0; i < validStates.length; ++i) {
          if (validStates[i] === statusByte) {
            allowNext = true; 
          }
        }
        // update the status for this container
        obj.data('buttonEnable', allowNext);
        
        return allowNext;
      }
      
      
      /**
       *  @function updateButton
       *  @param {object} button - the next button object
       *  @param {string} containerName - the name of the containers
       *
       */
      function updateButton(button, containerName) {
        var releaseButton = true,
            allDisabled = DISABLED;
        
        $("." + containerName).each(function () {
          if($(this).css('display') != 'none') {
            // check the buttonEnable variable that is attached to the container data
            if (!($(this).data('buttonEnable'))) {
              releaseButton = false;
            }
            // if we have all networks disabled, "allDisabled" will be true
            allDisabled = allDisabled & $(this).data('submitStatus');
          }
        });
        
        // special case all networks are disabled
        if (allDisabled) {
          releaseButton = false;
        } 
         
        if (releaseButton) {
          button.removeAttr('disabled');
        } else {
          button.attr('disabled', true);
        }
      }


      /**
       *	@function verifyPwdPassword
       *  @param {object} obj - the password field object
       *  @param {object} container - the outer container of the form segment
       *  @return  {Boolean} True if passwords match
       *  
       *  This function will compare a password with an adjacent password field
       *  which must have the class "verifyPwd". If the passwords match True is returned.
       *  It also updates the status flag VALID_PWD
       *
       *  Required html
       *    <li class="pwdInput">
       *    <label for="...">...</label>
       *    <input id="..." class="primaryPwd" type="password">
       *    <label for="...">verifyPwd Password:</label>
       *    <input id="..." class="verifyPwd" type="password">
       *    </li>
       *
       */
      function verifyPassword(obj, container) {
		  
        var thisPwdValue = obj.val(),
            verifyPwdValue = obj.siblings('.verifyPwd').val();
          
          // reset any prior error message
          // filter type="text" input created in jquery.placeholder.js for IE8 and IE9 
          obj.siblings('.verifyPwd').not('.placeholder').siblings('.error').remove();
          obj.siblings('.verifyPwd').not('.placeholder').removeClass('alert');
         

        if (thisPwdValue !== verifyPwdValue) {
          if (container || ($.trim(verifyPwdValue).length && !obj.siblings('.verifyPwd').is(':focus'))) {
            // add an error and alert
            // filter type="text" input created in jquery.placeholder.js for IE8 and IE9 
            obj.siblings('.verifyPwd').not('.placeholder').addClass('alert').after("<span class='error'><%getstr("pwd_no_match");%></span>");
          }
          
          // if this sub routine is used without a container then container will be empty
          // reset VALID_PWD 
          if (container)
            setStatusFlag(container, VALID_PWD, false);

          return false;
        } else {
        // set VALID_PWD
          if (container)
            setStatusFlag(container, VALID_PWD, true);

          return true;
        }
      }


      /**
       *  @function inputPasswordTextSwitch
       *  @param {object} obj - the input object with password or text type
       *  @param {Number} passwd_source - 0(text->password),1(password->text)
       *  @param {Number} focus_if - 0(focus not on the input object),1(focus on the input object)
       *
       *  This function is used to switch input type "password" and "text" in IE8. 
       *  Make sure the input object is the first input element in its parent element.
       */
      function inputPasswordTextSwitch(obj,passwd_source,focus_if) {
        var inputContent = obj.parent().html();
          if (inputContent != undefined) {
            inputContent = inputContent.substr(inputContent.toLowerCase().indexOf("<input"));
            if (passwd_source == 1) {
              inputContent = inputContent.substr(0,inputContent.indexOf(">")+1).replace("type=password","type=text").replace("value=\"\"","value=\""+obj.val()+"\"");
            } else {
              inputContent = inputContent.substr(0,inputContent.indexOf(">")+1).replace("type=text","type=password").replace("value=\"\"","value=\""+obj.val()+"\"");
            }
            $(obj).after($(inputContent));
            if (focus_if == 1) {
              obj.next().focus();
            }
            obj.remove();
          }
      }  
      function verifyEmail(obj) {
        var email= obj.val();
        var email_reg = /^\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*$/;  

        obj.siblings('.error').remove();
        obj.removeClass('alert');

        if (($.trim(email).length > MAX_EMAIL_CHARACTERS) && (!obj.is(':focus'))) {
          obj.addClass('alert').after("<span class='error'><%getstr("error_email_addr");%></span>");
          return false;
        }

        if ((!email_reg.test(email)) && (!obj.is(':focus'))) {
          obj.addClass('alert').after("<span class='error'><%getstr("error_email_addr");%></span>");
          return false;
        }

        return true;
      }

      /*******************************************************************************************
       *
       *  page resizing
       *  When a browser window resize occurs we force a page reload so the page is 
       *  initialized properly
       *
       ******************************************************************************************/
      $(window).resize (function () {
        if(this.resizeTO) {
          clearTimeout(this.resizeTO);
        }
        this.resizeTO = setTimeout(function() {
          $(this).trigger('resizeEnd');
        }, 300);
      });
      
      function reload_page() {
        if (($("input[name='cur_url']").length > 0) 
            && ($("input[name='cur_url']").val().length > 0))
          top.location.href = $("input[name='cur_url']").val();
        else
          document.location.reload();
      }
      
      $(window).on('resizeEnd', function () {
        /* Don't refresh page for every resize event, just do it when change 
         * from one mode to another (e.g., tablet to phone) */
        //document.location.reload();
        var windowWidth = $(window).innerWidth();
        if( isUnknown==true || currentWidth==windowWidth ) {
          /* doesn't have nav button, don't need to reload */
        }
        else if( (windowWidth<980)&&(windowWidth>=600)&&(isTablet==false) ) {
          reload_page();
        }
        else if( (windowWidth<600) && (isPhone==false) ) {
          reload_page();
        }
        else if( (isPhone==true||isTablet==true) && (windowWidth>=980) ) {
          reload_page();
        }
        else {
          currentWidth = windowWidth;
        }
      });
      
      
      
      /*******************************************************************************************
       *
       *  date/time picker widget
       *  time picker: https://github.com/jonthornton/jquery-timepicker
       *  date picker: http://www.kelvinluck.com/assets/jquery/datePicker/v2/demo/index.html
       *               http://www.kelvinluck.com/assets/jquery/datePicker/v2/demo/documentation.html
       *
       ******************************************************************************************/
      if ($('.timePicker').length) {
        if ("<%get_timepicker_format_hour_24();%>" == "1") {
          $('.timePicker').timepicker({
            disableTouchKeyboard: true,
            disableTextInput: true,
            timeFormat: 'H:i'
          });
        } else {
          $('.timePicker').timepicker({
            disableTouchKeyboard: true,
            disableTextInput: true,
            timeFormat: 'h:ia'
          });
        }
      }
      if ($('.datePicker').length) {
        Date.format = 'mm/dd/yyyy';     
        $('.datePicker').datePicker({
          startDate: '01/02/1970',
          clickInput:true,
          createButton:false
          
        });
        $('.datePicker').dpSetOffset(25, -20);
      }
      
      
      /*******************************************************************************************
       *
       *	modal popups
       *
       ******************************************************************************************/

      if ($('.modal').length) {

        var $modal = $('.modal'),
          $screen = "<div id='modalOverlay'></div>";
          // load the overlay 
        if(!$('#modalOverlay').length)
          $('body').append($screen);

        // attach click handler on every modal link on this page
        $modal.each(function (i) {

          // we use the link index to create a unique id for each modal box
          var $thisLink = $(this),
            linkIndex = i + 1;

          /**
           * this is the first click only, we build the modal box and load the content
           * any further click will be dealt with a different click handler
           */
          $thisLink.one('click', function (e) {

            // build the modal box
            var modalBoxID = 'modalBox' + linkIndex,
              $modalBox = $("<div id='" + modalBoxID + "' class='modalBox'><div class='loadingModal'></div></div>");

            // activate the overlay
            if($('#modalOverlay').hasClass('active'))
              $('#modalOverlay').fadeIn();
            else
              $('#modalOverlay').addClass('active');
            $('body').append($modalBox);
            
            // check if we are overwriting the width with a data attribute
            // if width exsists we overwrite the css
            if(typeof($thisLink.data('popup-type')) != "undefined" && $thisLink.data('popup-type') !== null) {
              
              $('#' + modalBoxID).addClass($thisLink.data('popup-type'));
              
            }
            
            // get the url from the link
            var modalBoxContentURL = $thisLink.attr('href'),
              $thisModalBox = $('#' + modalBoxID);

            // and load the content into the modal box
            $thisModalBox.load(modalBoxContentURL, function () {
              // attach a close handler
              $thisModalBox.find('.close').click(function () {
                $($thisModalBox).fadeOut();
                $('#modalOverlay').fadeOut();
              });
            }); // end of load content

            /**
             * this is the click handler for all subsequent clicks
             */
            $thisLink.click(function () {
              $('#modalOverlay').fadeIn();
              $($thisModalBox).fadeIn();
              return false;
            });

            return false;

          }); // end first click   
        }); // end each link
      }
		      
		
		
      /*******************************************************************************************
       *
       *	select networks page
       *
       ******************************************************************************************/

      if ($('.availableNetworks').length) {
      
        var nextButton = $('#nextStep');

        // reset for page refresh as firefox does not resets forms as all other browsers do
        //$('.tableControl').find(':checkbox').prop('checked', true);
        $('.activWrap').find(':radio').prop('checked', false);
        $('.activWrap').find(':text').val("");
        $('.activWrap').find(':password').val("");
        $('.activWrap').find('select').prop('selectedIndex', 0);
        
        
        // build the valid states array for the networks settings page
        validStates = [];
        validStates[0] = AUTO_SELECT | NOT_DFS;
        validStates[1] = NAME_EXISTS | MANUAL_SELECT | SEC_OPTION_NONE;
        validStates[2] = NAME_EXISTS | MANUAL_SELECT | SEC_OPTION | VALID_PWD;
        validStates[3] = DISABLED;
        
        $('.activWrap').each(function () {
          // on page load the network name is prepopulated and the password is set to
          // use existing... This is a valid state
          $(this).data('submitStatus', 0x00);
          $(this).data('buttonEnable', false);
        });
        
        //  update the next button 
        updateButton($('#nextStep'), 'activWrap');

        // disable the password verify inputs, they will be enabled once a valid password is input
        $('.verifyPwd').prop('disabled', true);
        

        /**
         *  page behavior
         */

        // make table active if associated checkbox is selected upon selection
        $('.tableControl').find(':checkbox.enable_network').on('change', function () {
        
          var thisContainer = $(this).parents('.tableControl').siblings('.activWrap');
          
          if ($(this).is(':checked')) {
            $(this)
              .parents('.tableControl')
              // make the table active
              .next()
              .removeClass('inactive');
            
            // reset DISABLED flag
            setStatusFlag(thisContainer, RESET_SELECT, false); 
              
          } else {
            // we disabled the table
            // reset all radio buttons, selects and password fields
            $(this)
              .parents('.tableControl')
              .next()
              .find(':input')
              .each(function () {
                var type = this.type,
                  tag = this.tagName.toLowerCase();
                if (type === 'text' || type === 'password') {
                  this.value = "";
                } else if (type === 'checkbox' || type === 'radio') {
                  this.checked = false;
                } else if (tag === 'select') {
                  this.selectedIndex = 0;
                }
              });
            // make the lists inactive and hide the manual input section	
            $(this)
              .parents('.tableControl')
              .next()
              .addClass('inactive')
              .find('.manualNetworkInput')
              .slideUp()
              .find('.pwdInput')
              .slideUp();

            // reset the select flags
            setStatusFlag(thisContainer, RESET_ALL, false);
            // set DISABLED flag
            setStatusFlag(thisContainer, DISABLED, true);
              
          }
          // update the button to reflect the new select status
          updateButton($('#nextStep'), 'activWrap');
        });
        
        // show SmartMesh enabled network only if associated checkbox is selected upon selection
        $('.tableControl').find(':checkbox.show_smesh').on('change', function () {
        
          var thisContainer = $(this).parents('.tableControl').siblings('.activWrap');
          
          if ($(this).is(':checked')) {
            $(this)
              .parents('.tableControl')
              .next()
              .find('.availableNetworks tbody tr:not(.smesh):visible')
              .addClass('hideNonSmesh');
          } else {
            $(this)
              .parents('.tableControl')
              .next()
              .find('.availableNetworks tbody tr')
              .removeClass('hideNonSmesh');
          }
          
          // reset the select flags
          setStatusFlag(thisContainer, RESET_ALL, false);
          
          // update the button to reflect the new select status
          updateButton($('#nextStep'), 'activWrap');
        });

        // manage network selection
        $('.availableNetworks').find(':radio').on('change', function (e) {

          var thisContainer = $(this).parents('.activWrap');

          // do not allow a radio button to be set if the table is inactive
          if ($(this).is(':checked') && $(this).parents('.activWrap').hasClass('inactive')) {
            this.checked = false;
            e.stopPropagation();
          } else {
            // we selected one of the prepopulated selections 
            // so reset the manual fields and hide the input pane
            $(this)
              .parents('.activWrap')
              .find('.manualNetworkInput')
              .find(':input')
              .each(function () {
                var type = this.type,
                  tag = this.tagName.toLowerCase();
                if (type === 'text' || type === 'password') {
                  this.value = "";
                } else if (type === 'checkbox' || type === 'radio') {
                  this.checked = false;
                } else if (tag === 'select') {
                  this.selectedIndex = 0;
                }
              });
            // any radio button selected in the table will hide the manual selection
            $(this)
              .parents('.availableNetworks')
              .siblings('.manualNetworkInput')
              .slideUp()
              .find('.pwdInput')
              .slideUp();
            
            setStatusFlag(thisContainer, RESET_ALL, false);
            setStatusFlag(thisContainer, AUTO_SELECT, true);

            if ($(this).parents('tr').hasClass('dfs')) {
              alert("<%getstr("DFS_radar_mark");%>");
              setStatusFlag(thisContainer, NOT_DFS, false);
            }
            else
              setStatusFlag(thisContainer, NOT_DFS, true);

            updateButton($('#nextStep'), 'activWrap');
          }
        });

        // manual network name input
        $('.manualSelect').find(':radio').on('change', function (e) { /* acos */
          
          var thisContainer = $(this).parents('.activWrap');
          
          // do not allow a radio button to be set if the table is inactive
          if ($(this).is(':checked') && $(this).parents('.activWrap').hasClass('inactive')) {
            this.checked = false;
            e.stopPropagation();
          } else {
            // we selected manual input, show the network name input and security protocol select
            $(this)
              .parents('.availableNetworks')
              .siblings('.manualNetworkInput')
              .slideUp()
              .find('.pwdInput')
              .slideUp();
            
            //  reset the auto select, we have selected manual input  
            setStatusFlag(thisContainer, RESET_ALL, false);
            setStatusFlag(thisContainer, AUTO_SELECT, true);
            setStatusFlag(thisContainer, NOT_DFS, true);
            updateButton($('#nextStep'), 'activWrap');
          }
        });
        
        /**
         *
         *  DEMO ONLY, remove for production <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
         *
         */
        $('.refreshBtn').click(function () { /* acos */
          //window.location.href="screen2-scan.html";
          //$(this).parents('.tableControl').next().find('.loadingMessage').toggle();
        });


      } // end $('.availableNetworks').length



      /*******************************************************************************************
       *
       *	enter password page
       *
       ******************************************************************************************/
      function check_enterPwd_page() {
        var releaseButton = true;
        $(".column").each(function () {
          if($(this).css('display') != 'none') {
            $(this).find('.networkName').each(function () {
              if ($.trim($(this).val()).length <= 0)
                releaseButton = false;
            });
            
            $(this).find('.securityOptions').each(function () {
              var securityOption = $(this).val();
              switch (securityOption) {
                case '0': // no option selected
                  releaseButton = false;
                  break;
                case '1':  // none selected
                case '2':  // WEP selected
                case '3':  // WPA-PSK [TKIP] selected
                case '4':  // WPA2-PSK [AES] selected
                case '5':  // WPA-PSK [TKIP] + WPA2-PSK [AES] selected
                  break;
              }
            });
            
            $(this).find('.pwdInput').each(function () {
              if($(this).css('display') != 'none') {
                if ($(this).find('.pwd').val().length <= 0)
                  releaseButton = false;
              }
            });
            
            $(this).find('.manual_wep').each(function () {
              if($(this).css('display') != 'none') {
                var wepenclen;
                var weptype = $(this).find('select[name^=wep_enc]');
                if (weptype.length) {
                  if (weptype.val() == "1")
                    wepenclen = 10;
                  else if (weptype.val() == "2")
                    wepenclen = 26;
                } else
                  wepenclen = 10;
                
                $(this).find('.KEY_manual').each(function () {
                  if ($(this).parent().find(':radio').prop('checked')) {
                    if (($.trim($(this).val()).length != wepenclen) && ($.trim($(this).val()).length != wepenclen/2))
                      releaseButton = false;
                  }
                });
              }
            });
          }
          
        });
        
        if (releaseButton)
          $('#nextStep').removeAttr('disabled');
        else
          $('#nextStep').attr('disabled', true);
      }
      
      if ($('.enterPwd').length) { /* acos */
      
        // select security options is revealed when the user selcts "use a different password"
        // this may be set to none or several WEP/WAP security options
        $('.securityOptions').change(function () {
          // if we select any value other than None (1) or the label Security Options (0)  
          // we will show a password field with verification 
          var securityOption = $(this).val(),
            thisContainer = $(this).parents('.manualNetworkInput'),
            tempStatus = 0x00;
          
          switch (securityOption) {
            
            case '0': // no option selected
              thisContainer.find('.pwdInput').slideUp();
              thisContainer.find('.manual_wep').slideUp();
              break;

            case '1':  // none selected
              thisContainer.find('.pwdInput').slideUp();
              tempStatus = SEC_OPTION_NONE;
              // reset the password fields
              thisContainer.find(':password').val('');
              thisContainer.find('.manual_wep').slideUp();
              break;

            case '2':  // WEP selected
              thisContainer.find('.manual_wep').slideDown();
              thisContainer.find('.pwdInput').slideUp();
              tempStatus = SEC_OPTION;
              break;
            case '3':  // WPA-PSK [TKIP] selected
            case '4':  // WPA2-PSK [AES] selected
            case '5':  // WPA-PSK [TKIP] + WPA2-PSK [AES] selected
              thisContainer.find('.pwdInput').slideDown();
              thisContainer.find('.manual_wep').slideUp();
              tempStatus = SEC_OPTION;
              break;
          }
        });
      
        // add the show password function
        // on checkbox click we adjust the password field type
        $('.showPwd').on('change', function () {
          var passwordInput = $(this).parents('.formElements').find('.pwd');
          if ($(this).is(':checked')) {
            // we change the type of the password field to text so we can see the password
            if(oldIE) {
              inputPasswordTextSwitch(passwordInput,1,0);
            }
            else{
              passwordInput.attr('type', 'text');
            }
          } else {
            if(oldIE) {
              inputPasswordTextSwitch(passwordInput,0,0);
            }
            else{
              passwordInput.attr('type', 'password');
            }
            }
            if(oldIE) {
              $('.pwd').keyup(function () {
              var pwdInput = $(this).val(),
              nextButton = $('#nextStep');
              // make sure we have valid input. we will ignore all spaces
              if ($.trim(pwdInput).length > 0) {
                nextButton.removeAttr('disabled');
              } else {
                nextButton.attr('disabled', true);
              }
            });
          }
        });
        
        // when the password field gets focus and the show password checkbox is enabled
        // we change the password field type to text
        $('.pwd').focus(function () {
          if ($(this).parents('.formElements').find('.showPwd').is(':checked')) {
            // we change the type of the password field to text so we can see the password
            if(oldIE) {
              inputPasswordTextSwitch($(this),1,1);
            }
            else
              $(this).attr('type', 'text');
          } else {
            if(oldIE) {
              inputPasswordTextSwitch($(this),0,1);
            }
            else
              $(this).attr('type', 'password');
          }
          if(oldIE) {
              $('.pwd').keyup(function () {
              var pwdInput = $(this).val(),
              nextButton = $('#nextStep');
              // make sure we have valid input. we will ignore all spaces
              if ($.trim(pwdInput).length > 0) {
                nextButton.removeAttr('disabled');
              } else {
                nextButton.attr('disabled', true);
              }
            });
          }
        });
        
        // initially the password fields are disabled. we enable them 
        // once a network name is added
        $('.networkName').keyup(function () {
          var str = $(this).val();
          // we check if string only contains spaces
          if ($.trim(str).length > 0) {
            // enable the password fields
            $(this)
              .parents('.manualNetworkInput')
              .find('.pwd')
              .removeAttr('disabled');
            $(this)
              .parents('.manualNetworkInput')
              .find('.KEY_manual')
              .removeAttr('disabled');
          } else {
            $(this)
              .parents('.manualNetworkInput')
              .find('.pwd')
              .attr('disabled', true);
            $(this)
              .parents('.manualNetworkInput')
              .find('.KEY_manual')
              .attr('disabled', true);
          }
        });
        
        $('.enterPwd').keyup(function () {
          check_enterPwd_page();
        });   
        $('.enterPwd').on('change', function () {
          check_enterPwd_page();
        });   

      } // end enterPwd



      /*******************************************************************************************
       *
       *	enter security settings page
       *
       ******************************************************************************************/

      if ($('.enterExtenderSettings').length || $('.enterAccessPointSettings').length) {
      
        /**
         *  The next button will only be enabled if a valid state of the two forms exists
         *  We are using status flags that in certain combinations will enable the next button
         */
        

        // build the valid states array for the access point settings page
        if ($("#ap_mode_flag").val() === '1') {
          validStates = [];
          validStates[0] = NAME_EXISTS | SEC_OPTION_NONE;
          validStates[1] = NAME_EXISTS | SEC_OPTION | VALID_PWD;
        }
        // build the valid states array for the extender settings page
        else {
          validStates = [];
          validStates[0] = NAME_EXISTS | SAME_PWD;
          validStates[1] = NAME_EXISTS | NO_PWD;
          validStates[2] = NAME_EXISTS | DIFF_PWD | SEC_OPTION_NONE;
          validStates[3] = NAME_EXISTS | DIFF_PWD | SEC_OPTION | VALID_PWD;
        }
            
        // disable the password verify inputs, they will be enabled once a valid password is input
        $('.verifyPwd').prop('disabled', true);
        
          
        //  Firefox doesn't refresh the form when page refresh. 
        //  Restore all form fields so FF behaves the same as all other browsers
        // this requires that the form has a name, normally same as the id
        if ($('#selectNetwork').length) {
          document.selectNetwork.reset();  
        }
        if ($('#accessPointSettings').length) {
          document.accessPointSettings.reset();  
        }   
        
        if ($("#ap_mode_flag").val() === '1') {
          $('.networkInput').each(function () {
            // on page load the network name is prepopulated and we need a password selection
            // so the next button will be disabled until a valid password selection has been made
            $(this).data('submitStatus', 0x80);
            $(this).data('buttonEnable', false);
          });
        }
        //  attach the status to each network container 
        else {
          $('.networkInput').each(function () {
            // on page load the network name is prepopulated and the password is set to
            // use existing... This is a valid state
            $(this).data('submitStatus', 0xc0);
            $(this).data('buttonEnable', true);  
          });
        }
        
        //  update the next button 
        updateButton($('#nextStep'), 'networkInput');
         
        
        
        
        
        /**
         *  page behavior
         */
        
        
        // the network name is pre populated but may be changed
        // this onbly checks if a name exists. may need more validation
        $('.networkName').blur(function () {
          var name = $(this).val(),
            thisContainer = $(this).parents('.networkInput');
          
          // if we don't have a name of the name is all spaces we reset NAME_EXISTS 
          // if we have a name we set the flag
          if (name.length === 0 || $.trim(name).length === 0) {
            // reset flag
            setStatusFlag(thisContainer, NAME_EXISTS, false);
          } else {
            // set flag
            setStatusFlag(thisContainer, NAME_EXISTS, true);
          }
          updateButton($('#nextStep'), 'networkInput');
        });
        
        
        // password selection is preset to "same as existing network password"
        // this may be changed to either none or different password
        $('.whatPwd').change(function () {
          var whatPwd = $(this).val(),
            thisContainer = $(this).parents('.networkInput'),
            tempStatus = 0x00;
          
          switch (whatPwd) {
          
          case '1':
            thisContainer.find('.diffPwd').slideDown();
            tempStatus = DIFF_PWD;
            break;
          
          case '0':
          case '2':
            if (whatPwd === '0') {
              tempStatus = SAME_PWD;
              // reset the sec options
              setStatusFlag(thisContainer, RESET_SEC_OPTIONS, false);
            }
            if (whatPwd === '2') {
              tempStatus = NO_PWD;
              // reset the sec options
              setStatusFlag(thisContainer, RESET_SEC_OPTIONS, false);
            }
            // hide the whole diffPwd pane
            thisContainer.find('.diffPwd').slideUp();
            // reset the security options select
            thisContainer.find('.securityOptions').prop('selectedIndex', 0);
            // hide the password pane
            thisContainer.find('.pwdInput').hide();
            thisContainer.find('.wepInput').hide();
            // reset the password fields
            thisContainer.find(':password').val('');
            // reset alert state and remove error messages
            thisContainer.find('.primaryPwd').next('span').remove();
            thisContainer.find('.primaryPwd').next('.pwdError').remove();
            thisContainer.find('.verifyPwd').removeClass('alert').prop('disabled', true).next().remove();
            break;
          }
          
          // reset the password type
          setStatusFlag(thisContainer, RESET_PWD_TYPE, false);
          // set the appropriate flag
          setStatusFlag(thisContainer, tempStatus, true);
          // update the button to reflect the new select status
          updateButton($('#nextStep'), 'networkInput');
        });
        
        
        
        // select security options is revealed when the user selcts "use a different password"
        // this may be set to none or several WEP/WAP security options
        $('.securityOptions').change(function () {
          // if we select any value other than None (1) or the label Security Options (0)  
          // we will show a password field with verification 
          var securityOption = $(this).val(),
            thisContainer = $(this).parents('.networkInput'),
            tempStatus = 0x00;
          
          switch (securityOption) {
          
          case '0': // no option selected
            thisContainer.find('.pwdInput').slideUp();
            thisContainer.find('.manual_wep').slideUp();
            break;

          case '1':  // none selected
            thisContainer.find('.pwdInput').slideUp();
            tempStatus = SEC_OPTION_NONE;
            // reset the password fields
            thisContainer.find(':password').val('');
            thisContainer.find('.manual_wep').slideUp();
            break;

          case '2':  // WEP selected
            thisContainer.find('.manual_wep').slideDown();
            thisContainer.find('.pwdInput').slideUp();
            tempStatus = SEC_OPTION;
            break;
          case '3':  // WPA-PSK [TKIP] selected
          case '4':  // WPA2-PSK [AES] selected
          case '5':  // WPA-PSK [TKIP] + WPA2-PSK [AES] selected
            thisContainer.find('.pwdInput').slideDown();
            thisContainer.find('.manual_wep').slideUp();
            tempStatus = SEC_OPTION;
            break;
          }
          
          switch (securityOption) {
          case '2':  // WEP selected
            break;
          case '3':  // WPA-PSK [TKIP] selected
            alert("<%getstr("alert_wpapsk");%>");
            break;
          case '5':  // WPA-PSK [TKIP] + WPA2-PSK [AES] selected
            alert("<%getstr("alert_autopsk");%>");
            break;
          }
          
          // reset the password type
          setStatusFlag(thisContainer, RESET_SEC_OPTIONS, false);
          // set the appropriate flag
          setStatusFlag(thisContainer, tempStatus, true);
          // update the button to reflect the new select status
          updateButton($('#nextStep'), 'networkInput');
        });
      
      
      
        // add the password strength indicator to each password field instance
        $(".pwdInput").find(':password').each(function () {
          if ($(this).hasClass('primaryPwd')) {
            var networkName = '#' + $(this).parents('.formElements').find('.networkName').attr('id');

            // each password field gets an instance of the strength indicator
            $(this).passStrength({
              shortPass: "top_shortPass",
              longPass: "top_longPass",
              badPass: "top_badPass",
              goodPass: "top_goodPass",
              strongPass: "top_strongPass",
              baseStyle: "top_testresult",
              userid: networkName,
              messageloc: 1,
              minLength: MIN_PWD_CHARACTERS,
              maxLength: MAX_PWD_CHARACTERS
            });
          }
        });

      
        /**
         * the password should be equal or greater than MIN_PWD_CHARACTERS and should
         * not have any leading or trailing spaces
         */
        
        
        // on password input blur we check if the password is >= 8 characters
        // if >= 8 chars we enable the verify input
        // if < 8 chars we show an error message
        $('.primaryPwd').blur(function () {
          var pwd = $(this).val(),
            thisContainer = $(this).parents('.networkInput');
          
          if ($.trim(pwd).length < MIN_PWD_CHARACTERS || $.trim(pwd).length > MAX_PWD_CHARACTERS) {
            // reset flag
            setStatusFlag(thisContainer, VALID_PWD, false);
            updateButton($('#nextStep'), 'networkInput');
            
            $(this).next().after("<p class='pwdError'><%getstr("passphrase_notice");%></p>");
          } else {
            // we have a valid password enable the verify input field
            $(this).siblings('.verifyPwd').prop('disabled', false);
          }          
        });
        
        // with each keyup event we check if >= 8 chars
        // if > 8 chars we enable the verify input
        $('.primaryPwd').keyup(function () {
          var pwd = $(this).val();
          if ($.trim(pwd).length >= MIN_PWD_CHARACTERS && $.trim(pwd).length <= MAX_PWD_CHARACTERS) {
            // we have a valid password enable the verify input field
            $(this).siblings('.verifyPwd').prop('disabled', false);
          } else {
            $(this).siblings('.verifyPwd').prop('disabled', true);
          }
        });
        
        // on password input focus we reset any prior eror messages
        // and also reset the verify input. However, we leave the verify input
        // enabled if the password input has > 7 chars. That will be the case
        // if the user returns to the input  
        $('.primaryPwd').focus(function () {
          var pwd = $(this).val(),
            thisContainer = $(this).parents('.networkInput');
          // remove prior pwd error message
          $(this).siblings('.pwdError').remove();
          
          // reset the verify input. we do not need to disable verify as the password is still
          // in valid range.
          $(this).siblings('.verifyPwd').val('').removeClass('alert');
          $(this).siblings('.error').remove();
        });
        /*
        // reset any prior errors
        $('.verifyPwd').focus(function () {
          $(this).removeClass('alert').siblings('.error').remove();
        });
        */
        // verify password
        $('.verifyPwd').keyup(function () {
          var thisContainer = $(this).parents('.networkInput');
          verifyPassword($(this).parent().find('.primaryPwd'), thisContainer);
          updateButton($('#nextStep'), 'networkInput');
        });
        

        $('.KEY_manual').keyup(function () {
          var pwdInput = $(this).val(),
              thisContainer = $(this).parents('.networkInput');
          var wepenclen;
            //nextButton = $('#nextStep');
          var weptype = $(this).parents(".manual_wep").find('select[name^=wep_enc]');
          if (weptype.val() == "1")
            wepenclen = 10;
          else if (weptype.val() == "2")
            wepenclen = 26;
              
          if (($.trim(pwdInput).length == wepenclen) || ($.trim(pwdInput).length == wepenclen/2)) {
            //nextButton.removeAttr('disabled');
            setStatusFlag(thisContainer, VALID_PWD, true);
          } else {
            //nextButton.attr('disabled', true);
            setStatusFlag(thisContainer, VALID_PWD, false);
          }
          
          updateButton($('#nextStep'), 'networkInput');
        });
      } // end enterSecuritySettings

      function check_registration() {
        var nextButton = $('#finishBt'),
          FIRST_NAME     = 0x80,
          LAST_NAME        = 0x40,
          EMAIL            = 0x20,
          PURCHASE_DATE    = 0x10,
          SERIAL_NUMBER    = 0x08,
          status           = 0x00,
          ALL_SET          = 0xf8;

        if ($.trim($('#firstName').val()).length > 0)
          status = status | FIRST_NAME;

        if ($.trim($('#lastName').val()).length > 0)
          status = status | LAST_NAME;

        if ($.trim($('#email').val()).length > 0 && verifyEmail($('#email')))
          status = status | EMAIL;

        if ($.trim($('#purchaseDate').val()).length > 0)
          status = status | PURCHASE_DATE;

        if ($.trim($('#serialNumber').val()).length > 0)
          status = status | SERIAL_NUMBER;

        if ($('#refurbished').is(':checked'))
          status = status | SERIAL_NUMBER;

        if (status === ALL_SET)
          nextButton.removeAttr('disabled');
        else
          nextButton.attr('disabled', true);
      }

      /*******************************************************************************************
       *
       *	registration page
       *
       ******************************************************************************************/

      if ($('.registration').length) {
        // reset the form for a common browser response
        //$(':text').val(""); /* acos */
        $(':checkbox').prop('checked', false);

        // checkbox for Bookmark NETGEAR Genie and Send me email are preset
        $(":checkbox[value='bookmark']").prop('checked', true);
        $(":checkbox[value='sendEmail']").prop('checked', true);

        check_registration();

        $('#firstName').keyup(function () {
          check_registration();
        });

        $('#lastName').keyup(function () {
          check_registration();
        });

        $('#email').keyup(function () {
          check_registration();
        });

        $('#purchaseDate').on('change', function () { /* acos */
          check_registration();
        });

        $('#serialNumber').keyup(function () {
          check_registration();
        });

        $('#refurbished').on('change', function () {
          check_registration();
        });
      } // end registration page
      
      
      /*******************************************************************************************
       *
       *	sidebar navigation
       *
       ******************************************************************************************/
      if ($('.mainNav').length) {
        
        $('.mainNav').find('.expanded').find('ul').slideDown();
        
        $('.mainNav').find('ul').prev().on('touchclick', function () {
        
          if ($(this).parent().hasClass('collapsed')) {
            $(this).parent()
              .removeClass('collapsed')
              .addClass('expanded')
              .find('i')
              .removeClass('icon-collapsed')
              .addClass('icon-expanded')
              .end()
              .find('ul')
              .slideDown();
          } else {
            $(this).parent()
              .removeClass('expanded')
              .addClass('collapsed')
              .find('i')
              .removeClass('icon-expanded')
              .addClass('icon-collapsed')
              .end()
              .find('ul')
              .slideUp();
          }
        });      
      }
      
      
      /*******************************************************************************************
       *
       *	footer search
       *
       ******************************************************************************************/

      if ($('footer').find('.search').length) {
      
    	  // move label into the placeholder attribute
    	  $('footer').find('.search').find('input').each(function () {
    	    var inputType = $(this).attr('type'),
    	        labelText;

  	      // get the label text
  	      labelText = $(this).siblings('label').text();
  	      // and insert it as a plceholder
  	      $(this).attr('placeholder', labelText);
  	      // now hide the label
  	      $(this).siblings('label').hide();
    	    
    	  });
      }
      
      
      /*******************************************************************************************
       *
       *	tablet/phone navigation
       *
       *  based on an inconsistent nav design we have to accommodate three different nav
       *  scenarios: Regular, medium width and small width.
       *
       ******************************************************************************************/
      if (isTablet || isPhone) {
        if ($('#country').length) {
          $('#country').find('>a').on('touchclick', function () {
            if ($(this).hasClass('open')) {
              $(this).removeClass('open');
              $(this).next().slideUp();
            } else {
              $(this).addClass('open');
              $(this).next().slideDown();
            }   
          });
        }
        
      if (isTablet) {
        if ($('#navButton').length) {        
          $('#navButton').on('touchclick', function () {
            if ($(this).hasClass('open')) {
              $(this).removeClass('open');
              $('#slider').css('marginLeft', '0');
            } else {
              $(this).addClass('open');
              $('#slider').css('marginLeft', '40%');
            }          
          });
        }
       }
       
      if (isPhone) {
        if ($('#navButton').length) {        
          $('#navButton').on('touchclick', function () {
            if ($(this).hasClass('open')) {
              $(this).removeClass('open');
              $('.mainNavWrap').slideUp();
            } else {
              $(this).addClass('open');
              $('.mainNavWrap').slideDown();
            }          
          });
        }
       }
      } 
      
      
      /*******************************************************************************************
       *
       *	phone table cell labels
       *  on a phone we present tables of type "devicesList in a linear form. we hide the table  
       *  header and insert label into each cell with the header text. 
       *
       ******************************************************************************************/
      if (isPhone && $('.devicesList').length) {
      
        $('.devicesList').find('td').each( function () {
          // get the corresponding table header
          var $th = $(this).parents('table').find('th').eq($(this).index());
          // add a label ionto the table cell
          $(this).prepend("<span class='tdLabel'>" + $th.text() + ":</span>");
        });    
      }
      
      
      
      /*******************************************************************************************
       *
       *	available networks listing on a mobile phone will only show first 3 and a see more link
       *
       ******************************************************************************************/
      if ($('.availableNetworks').length) {
      
        $('.availableNetworks').each( function () {
          // check each network list
          $(this).find('tr').each( function (i) {
            if (i > 3) {
              // add helper class so we can hide all found networks > 3
              $(this).addClass('hideOnMobile');
            }
            if (i === 3) {
              // add the see all link
              $(this).after("<tr class='moreLink'><td><a><%getstr("see_more");%></a></td></tr>");
            }
          });
        });
        
        // show and hide the row >3
        // notice that when showing the rows jQuery assigns display table cell
        // so we will make these rows visible by asigning display block to the tds
        // is is needed for the phone layout
        $('.availableNetworks').find('.moreLink').find('a').on('touchstart', function () {
          if ($(this).hasClass('open')) {
            $(this)
              .removeClass('open')
              .text("<%getstr("see_more");%>")
              .parents('.availableNetworks')
              .find('.hideOnMobile')
              // hide the tds as webkit doesn't hide trs
              .each( function () {
                $(this).find('td').hide();
              });  
            } else {
            $(this)
              .addClass('open')
              .text("<%getstr("see_less");%>")
              .parents('.availableNetworks')
              .find('.hideOnMobile')
              .each( function () {
                $(this)
                  .find('td')
                  .each( function (i) {
                    if (i < 2) {
                      $(this).css('display', 'block');
                    }
                  });
              });
            }
          });
      
        } // end available networks
      
      
        /*******************************************************************************************
         *
         *	WiFi Settings page
         *
         ******************************************************************************************/
        if ($('.wifiSettings').length) {
        
          //  Firefox doesn't refresh the form when page refresh. 
          //  Restore all form fields so FF behaves the same as all other browsers
          // this requires that the form has a name, normally same as the id
          document.mainForm.reset();
        
          // toggle the adcanced settings in pane General
          $('#toggleAdvancedSettings').on ('touchclick', function (){
            if ($(this).hasClass('showAdvSettings')) {
              $(this)
                .removeClass('showAdvSettings')
                .text("<%getstr("adv_settings");%>")
                .parents('.sectionDetails')
                .find('.advancedSettings')
                .hide();
            } else {
              $(this)
                .addClass('showAdvSettings')
                .text("<%getstr("bas_adv");%>")
                .parents('.sectionDetails')
                .find('.advancedSettings')
                .show();
            }
          });
        
        
          // toggle the various settings panes
          $('.sectionWrap').find('h2').on('touchclick', function () {
            if ($(this).parents('.sectionWrap').hasClass('open')) {
              $(this)
                .find('i')
                .removeClass('icon-expanded')
                .addClass('icon-collapsed')
                .parents('.sectionWrap')
                .removeClass('open')
                .find('.sectionDetails')
                .slideUp();
            } else {
              $(this)
                .find('i')
                .removeClass('icon-collapsed')
                .addClass('icon-expanded')
                .parents('.sectionWrap')
                .addClass('open')
                .find('.sectionDetails')
                .slideDown();
            }
          });
          
        
          // manage visibility of the password input fields
          // the password selection is set to same as existing network as default
          // hide the associated field until this is changed
          $('.manualNetworkInput').find('.securityOptionsWrap').hide();
          $('.manualNetworkInput').find('.pwdInput').hide();
          $('.manualNetworkInput').find('.wepInput').hide();
          
        
          // change of the password option will reveal the associated fields
          $('.whatPwd').on('change', function () {
          
            var thisParent = $(this).parents('.manualNetworkInput');
          
            if ($(this).val() === '1') {
              thisParent.find('.securityOptionsWrap').slideDown();
            } else {
              thisParent.find('.securityOptionsWrap').slideUp();
              // we have selected same or none password, we'll reset all password releated selections 
              thisParent.find('.securityOptions').prop('selectedIndex', 0);
              // hide the password pane
              thisParent.find('.pwdInput').hide();
              thisParent.find('.wepInput').hide();
              // reset the password fields
              thisParent.find('.pwdInput').find(':password').val('');  
            }         
          });
          /*
          $('.securityOptions').on('change', function () {
          
            var thisParent = $(this).parents('.manualNetworkInput');
            
            if ($(this).val() > '1') {
              thisParent.find('.pwdInput').slideDown();
            } else {
              // hide the password pane
              thisParent.find('.pwdInput').slideUp();
              // reset the password fields
              thisParent.find('.pwdInput').find(':password').val('');
            }   
          });
          */
          
          $('.wepPwd').keyup(function () {
            var wepPwd = $(this).val(),
              nextButton = $(this).siblings('.btn');
            if (wepPwd.length > 0) {
              nextButton.attr('disabled', false);
            } else {
              nextButton.attr('disabled', true);
            }
          });

          // add the password strength indicator to each password field instance
          $('.manualNetworkInput').on('focus', '.primaryPwd', function() {
            var networkName = '#' + $(this).parents('.formElements').find('.networkName').attr('id');

            // each password field gets an instance of the strength indicator
            $(this).passStrength({
              shortPass: "top_shortPass",
              longPass: "top_longPass",
              badPass: "top_badPass",
              goodPass: "top_goodPass",
              strongPass: "top_strongPass",
              baseStyle: "top_testresult",
              userid: networkName,
              messageloc: 1,
              minLength: MIN_PWD_CHARACTERS,
              maxLength: MAX_PWD_CHARACTERS
            });

            // remove prior pwd error message
            $(this).siblings('.pwdError').remove();
            
            // reset the verify input. we do not need to disable verify as the password is still
            // in valid range.
            $(this).siblings('.verifyPwd').val('').removeClass('alert');
            $(this).siblings('.error').remove();
          });

          $('.manualNetworkInput').on('blur', '.primaryPwd', function() {
            var pwd = $(this).val();
            if ($.trim(pwd).length < MIN_PWD_CHARACTERS || $.trim(pwd).length > MAX_PWD_CHARACTERS) {
              $(this).next().after("<p class='pwdError'><%getstr("passphrase_notice");%></p>");
            } else {
              // we have a valid password enable the verify input field
              $(this).siblings('.verifyPwd').prop('disabled', false);
            }
          });

          // with each keyup event we check if >= 8 chars
          // if > 8 chars we enable the verify input
          $('.manualNetworkInput').on('keyup', '.primaryPwd', function() {
            var pwd = $(this).val();
            if ($.trim(pwd).length >= MIN_PWD_CHARACTERS && $.trim(pwd).length <= MAX_PWD_CHARACTERS) {
              // we have a valid password enable the verify input field
              $(this).siblings('.verifyPwd').prop('disabled', false);
            } else {
              $(this).siblings('.verifyPwd').prop('disabled', true);
            }
          });

          // verify password
          $('.verifyPwd').keyup(function () {
            var thisContainer = $(this).parents('.formElements');
            verifyPassword($(this).parent().find('.primaryPwd'), thisContainer);
          });
        
          // add the show password function
          // on checkbox click we adjust the password field type
          $('.showPwd').on('change', function () {
            var passwordInput = $(this).parents('.pwdInput').find('.primaryPwd');
            if ($(this).is(':checked')) {
              // we change the type of the password field to text so we can see the password
              if(oldIE) {
                inputPasswordTextSwitch(passwordInput,1,0);
              } else {
                passwordInput.attr('type', 'text');
              }
            } else {
              if(oldIE) {
                inputPasswordTextSwitch(passwordInput,0,0);
              } else {
                passwordInput.attr('type', 'password');
             }
            }
          });

        } // end wifi settings
      
      
        /*******************************************************************************************
         *
         *	Editable table rows
         *
         ******************************************************************************************/
        if ($('.editDevice').length) {
          // edit a connected device
          $('.devicesList').find('.editDevice').each( function (i) {
            // click handler for each edit icon
            $(this).on('touchclick', function (){
              if ($(this).hasClass('editMode')) {
                // NOTE: the form must have autocomplete="off" otherwise the fields will not
                // return to proper state after a page reset for FF
                // reset all inputs to their default values
                $(this).parents('tr').find(':text').each( function () {
                    this.value = this.defaultValue;
                });
                
                $(this)
                  .removeClass('editMode')
                  .html('<i class="icon-edit"></i>')
                  .parents('tr')
                  .find(':text')
                  .prop('disabled', true);
                  
                  $(this)
                  .parents('tr')
                  .find('select')
                  .prop('disabled', true); 
                  
                  $(this)
                  .parents('tr')
                  .find(':button')
                  .prop('disabled', true);
                  
             } else {
                // make values available to be changed
                  $(this)
                  .addClass('editMode')
                  .html('<%getstr("cancel_mark");%>')
                  .parents('tr')
                  .find(':text')
                  .prop('disabled', false);
                  
                  $(this)
                  .parents('tr')
                  .find('select')
                  .prop('disabled', false);
                  
                  $(this)
                  .parents('tr')
                  .find(':button')
                  .prop('disabled', false);
              }
            });
          });
          
          // delete a table row with the cancel button
          $('.devicesList').on('click', '.cancelDevice', function (){
              $(this).parents('tr').remove();
          });
        }
      
       /*******************************************************************************************
        *
        *	USB page
        *
        ******************************************************************************************/
       if ($('.usbPort').length) {
         //  Firefox doesn't refresh the form when page refresh. 
         //  Restore all form fields so FF behaves the same as all other browsers
         // this requires that the form has a name, normally same as the id
         document.usbPortForm.reset();
         
         // show/hide the usb settings
         $("#usbOn").on('change', function () {
           $('#usbOffWrap').slideDown();
           $('#selSettings').fadeIn();
         });       
         
         $("#usbOff").on('change', function () {
           $('#usbOffWrap').slideUp();
           $('#selSettings').fadeOut();
         });
          
         // show/hide the printer sharing message
         $("#sharePrinter").on('change', function () {
           $('#sharePrinterWrap').slideUp();
           $('#printerShareInfo').slideDown();
           $('#selSettings').fadeOut();
         });
        
         $("#shareContent").on('change', function () {
           $('#sharePrinterWrap').slideDown();
           $('#printerShareInfo').slideUp();
           $('#selSettings').fadeIn();
         });
         
         // hide advanced settings initially
         $('#workgroupWrap').hide();
        
         // toggle the workgroup settings
         $('#selSettings').on ('touchclick', function (){
           if ($(this).hasClass('showAdvSettings')) {
             $(this)
               .removeClass('showAdvSettings')
               .text("<%getstr("adv_settings");%>")
               .parents('#usbPortForm')
               .find('#workgroupWrap')
               .slideUp();
           } else {
             $(this)
               .addClass('showAdvSettings')
               .text("<%getstr("bas_adv");%>")
               .parents('#usbPortForm')
               .find('#workgroupWrap')
               .slideDown();
           }
         });
        
         // add a folder to the connected devices table
         $('#addFolderBt').on('touchclick', function (){
         
           if ($("#no_usb_device").val() === '1')
             alert("<%getstr("no_usbdish");%>");
           else {
             // how many rows are there
             var numberOfRows = $(this).parent().prev('.devicesList').find('tbody').find('tr').length,
                 rowNumber = numberOfRows + 1,
                 newRowContent = "";
             if (numberOfRows >= 256)
               alert("<%getstr("share_folder_full");%>");
             else {
               // build the row html
               var addFolderNum = $("input[name^='add_folder_name'][id^='addFolderName']").length;
               addFolderNum = addFolderNum + 1;
               newRowContent += "<tr>";
               newRowContent += "<td><span class='tdLabel'>Folder Name:</span><input type='button' id='addFolderName" + addFolderNum + "' class='btn roundCorners controlBtn' style='width:100%; margin: 0px auto;'onClick='add_folder();' name='add_folder_name" + addFolderNum +  "' value='<%getstr("select_folder");%>'></td>";
               newRowContent += "<td><span class='tdLabel'>Folder Address:</span><input id='addFolderAddress" + addFolderNum + "' type='text' size='21' maxlength='20' name='add_folder_address" + addFolderNum + "' value=''></td>";
               newRowContent += "<td><span class='tdLabel'>Read Access:</span><select id='addReadAccess" + addFolderNum + "' name='add_read_access" + addFolderNum + "'><option selected value='1'><%getstr("pwd_off");%></option><option value='2'><%getstr("pwd_on");%></option></select></td>";
               newRowContent += "<td><span class='tdLabel'>Write Access:</span><select id='addWriteAccess" + addFolderNum + "' name='add_write_access" + addFolderNum + "'><option selected value='1'><%getstr("pwd_off");%></option><option value='2'><%getstr("pwd_on");%></option></select></td>";
               newRowContent += "<td><span class='tdLabel'>Total Space:</span><span class='tdLabel'>Enable:</span><input type='text' name='total_space" + rowNumber + "' value='--' disabled></td>";
               newRowContent += "<td><span class='tdLabel'>Available Space:</span><input type='text' name='avail_space" + rowNumber + "' value='--' disabled></td>";
               newRowContent += "<td class='cancelNewFolder' colspan=2><span class='tdLabel'>Edit:</span><a id='addCancelBt" + addFolderNum + "' class='editDevice editMode cancelDevice'><%getstr("cancel_mark");%></a></td>";
               newRowContent += "</tr>";
             
               $(this).parent().prev('.devicesList').find('tbody').append(newRowContent);
             }
           }
         });
       } // end USB Page
      
      function check_pwd_settings() {
        var nextButton = $('#saveBt'),
          EMAIL            = 0x08,
          PWD              = 0x04,
          ANSWER1          = 0x02,
          ANSWER2          = 0x01,
          status           = 0x00,
          ALL_SET          = 0x0f;
        
        if ($.trim($('.emailAddr').val()).length > 0)
          status = status | EMAIL;
        
        if ($.trim($('.primaryPwd').val()).length > 0
            && verifyPassword($('.verifyPwd').parent().find('.primaryPwd')))
          status = status | PWD;
        
        if ($.trim($("select[name='question1']").val()).length > 0
            && $.trim($("input[name='answer1']").val()).length > 0)
          status = status | ANSWER1;
        
        if ($.trim($("select[name='question2']").val()).length > 0
            && $.trim($("input[name='answer2']").val()).length > 0)
          status = status | ANSWER2;
        
        if ($("#pwdOff").prop('checked'))
          status = status | ALL_SET;
        
        if (status === ALL_SET)
          nextButton.removeAttr('disabled');
        else
          nextButton.attr('disabled', true);
      }
      
      /*******************************************************************************************
       *
       *	Password settings page
       *
       ******************************************************************************************/
      if ($('.pwdSettings').length) {
        check_pwd_settings();
        
        // show/hide the password settings
        $("#pwdOn").on('change', function () {
          $('.pwdSettingsOptions').slideDown();
          check_pwd_settings();
        });
        
        $("#pwdOff").on('change', function () {
          $('.pwdSettingsOptions').slideUp();
          check_pwd_settings();
        });
        
        // with each keyup event we check email
        $('.emailAddr').keyup(function () {
          check_pwd_settings();
        });

        // with each blur event we check email
        $('.emailAddr').blur(function () {
          check_pwd_settings();
        });

        // add the password strength indicator to each password field instance
        $(".pwdInput").find(':password').each(function () {
          if ($(this).hasClass('primaryPwd')) {
            var networkName = '#' + $(this).parents('.formElements').find('#userName').attr('id');
  
            // each password field gets an instance of the strength indicator
            $(this).passStrength({
              shortPass: "top_shortPass",
              longPass: "top_longPass",
              badPass: "top_badPass",
              goodPass: "top_goodPass",
              strongPass: "top_strongPass",
              baseStyle: "top_testresult",
              userid: networkName,
              messageloc: 1,
              minLength: MIN_USERPWD_CHARACTERS,
              maxLength: MAX_USERPWD_CHARACTERS
            });
          }
        });
  
      
        /**
         * the password should be equal or greater than MIN_USERPWD_CHARACTERS and should
         * not have any leading or trailing spaces
         */
        
        
        // on password input blur we check if the password is >= 8 characters
        // if >= 8 chars we enable the verify input
        // if < 8 chars we show an error message
        $('.primaryPwd').blur(function () {
          var pwd = $(this).val();
          //if ($.trim(pwd).length < MIN_USERPWD_CHARACTERS || $.trim(pwd).length > MAX_USERPWD_CHARACTERS) {
	      if(0) {
            $(this).next().after("<p class='pwdError'><%getstr("pwd_error_1");%>" + MIN_USERPWD_CHARACTERS + "-" + MAX_USERPWD_CHARACTERS + "<%getstr("pwd_error_2");%></p>");
          }
          
          check_pwd_settings();
        });
        
        // with each keyup event we check if >= 8 chars
        // if > 8 chars we enable the verify input
        $('.primaryPwd').keyup(function () {
          var pwd = $(this).val();
          //if ($.trim(pwd).length >= MIN_USERPWD_CHARACTERS && $.trim(pwd).length <= MAX_USERPWD_CHARACTERS) {
	      if(0) {
            // we have a valid password enable the verify input field
            $(this).siblings('.verifyPwd').prop('disabled', false);
          } else {
            $(this).siblings('.verifyPwd').prop('disabled', true);
          }
          
          check_pwd_settings();
        });
        
        // on password input focus we reset any prior eror messages
        // and also reset the verify input. However, we leave the verify input
        // enabled if the password input has > 7 chars. That will be the case
        // if the user returns to the input  
        $('.primaryPwd').focus(function () {
          // remove prior pwd error message
          $(this).siblings('.pwdError').remove();
          // reset the verify input. we do not need to disable verify as the password is still
          // in valid range.
          $(this).siblings('.verifyPwd').val('').removeClass('alert');
          $(this).siblings('.error').remove();
          
          check_pwd_settings();
        });
             // verify password
        $('.verifyPwd').keyup(function () {
          check_pwd_settings();
        });
        
        $('.verifyPwd').blur(function () {
          check_pwd_settings();
        });
        $("input[name='answer1']").keyup(function () {
          check_pwd_settings();
        });
        $("input[name='answer1']").blur(function () {
          check_pwd_settings();
        });
        
        $("input[name='answer2']").keyup(function () {
          check_pwd_settings();
        });
        $("input[name='answer2']").blur(function () {
          check_pwd_settings();
        });
        
        $("select[name='question1']").change(function () {
          check_pwd_settings();
        });
        
        $("select[name='question2']").change(function () {
          check_pwd_settings();
        });
      } // end password settings page
      
      function check_login_setup() {
        var nextButton = $('#nextStep'),
          EMAIL            = 0x08,
          PWD              = 0x04,
          ANSWER1          = 0x02,
          ANSWER2          = 0x01,
          status           = 0x00,
          ALL_SET          = 0x0f;
        
        if ($.trim($('.emailAddr').val()).length > 0)
          status = status | EMAIL;
        
        if ($.trim($('.primaryPwd').val()).length > 0
            && verifyPassword($('.verifyPwd').parent().find('.primaryPwd')))
          status = status | PWD;
        
        if ($.trim($("select[name='question1']").val()).length > 0
            && $.trim($("input[name='answer1']").val()).length > 0)
          status = status | ANSWER1;
        
        if ($.trim($("select[name='question2']").val()).length > 0
            && $.trim($("input[name='answer2']").val()).length > 0)
          status = status | ANSWER2;
        
        if (status === ALL_SET)
          nextButton.removeAttr('disabled');
        else
          nextButton.attr('disabled', true);
      }
      function check_login(){
        if ($.trim($('#userId').val()).length > 0)
        {
            $('#userId').siblings('.error').remove();
            return true;
        }
      }
      /*******************************************************************************************
       *
       *	web login page
       *
       ******************************************************************************************/
      if ($('.weblogin').length) { /* acos */
        // with each keyup event we check email
        $('#userId').keyup(function () {
          check_login();
        });
        // with each blur event we check email
        $('#userId').blur(function () {
          check_login();
        });
        
        // with each input event we check email
        $('#userId').on('input',function () {
          check_login();
        });
      
        $('#password').blur(function () {
          var pwd = $(this).val();
          $(this).siblings('.error').remove();
          $(this).removeClass('alert');
          //if ($.trim(pwd).length < MIN_USERPWD_CHARACTERS || $.trim(pwd).length > MAX_USERPWD_CHARACTERS) {
		  if(0) {
            $(this).next().after("<p class='error'><%getstr("pwd_error_1");%>" + MIN_USERPWD_CHARACTERS + "-" + MAX_USERPWD_CHARACTERS + "<%getstr("pwd_error_2");%></p>");
          }        
        });
      } // end web login page
      
      /*******************************************************************************************
       *
       *	web login setup page
       *
       ******************************************************************************************/
      if ($('.webloginSetup').length) { /* acos */
        // with each keyup event we check email
        $('.emailAddr').keyup(function () {
          check_login_setup();
        });

        // with each blur event we check email
        $('.emailAddr').blur(function () {
          check_login_setup();
        });

        // add the password strength indicator to each password field instance
        $(".pwdInput").find(':password').each(function () {
          if ($(this).hasClass('primaryPwd')) {
            var networkName = '#' + $(this).parents('form').find('#newUserName').attr('id');
            // each password field gets an instance of the strength indicator
            $(this).passStrength({
              shortPass: "top_shortPass",
              longPass: "top_longPass",
              badPass: "top_badPass",
              goodPass: "top_goodPass",
              strongPass: "top_strongPass",
              baseStyle: "top_testresult",
              userid: networkName,
              messageloc: 1,
              minLength: MIN_USERPWD_CHARACTERS,
              maxLength: MAX_USERPWD_CHARACTERS
            });
          }
        });
  
      
        /**
         * the password should be equal or greater than MIN_USERPWD_CHARACTERS and should
         * not have any leading or trailing spaces
         */
        
        
        // on password input blur we check if the password is >= 8 characters
        // if >= 8 chars we enable the verify input
        // if < 8 chars we show an error message
        $('.primaryPwd').blur(function () {
          var pwd = $(this).val();
          var obj = $(this).next();

          if($(this).next(".tipImage").length) 
            obj = $(this).next().next();
          //if ($.trim(pwd).length < MIN_USERPWD_CHARACTERS || $.trim(pwd).length > MAX_USERPWD_CHARACTERS) {
	      if(0) {
            obj.after("<p class='pwdError'><%getstr("pwd_error_1");%>" + MIN_USERPWD_CHARACTERS + "-" + MAX_USERPWD_CHARACTERS + "<%getstr("pwd_error_2");%></p>");
          }
          
          check_login_setup();
        });
        
        // with each keyup event we check if >= 8 chars
        // if > 8 chars we enable the verify input
        $('.primaryPwd').keyup(function () {
          var pwd = $(this).val();

          //if ($.trim(pwd).length >= MIN_USERPWD_CHARACTERS && $.trim(pwd).length <= MAX_USERPWD_CHARACTERS) {
	      if(0) {
            // we have a valid password enable the verify input field
            $(this).siblings('.verifyPwd').prop('disabled', false);
          } else {
            $(this).siblings('.verifyPwd').prop('disabled', true);
          }
          
          check_login_setup();
        });
        
        // on password input focus we reset any prior eror messages
        // and also reset the verify input. However, we leave the verify input
        // enabled if the password input has > 7 chars. That will be the case
        // if the user returns to the input  
        $('.primaryPwd').focus(function () {
          // remove prior pwd error message
          $(this).siblings('.pwdError').remove();
          // reset the verify input. we do not need to disable verify as the password is still
          // in valid range.
          $(this).siblings('.verifyPwd').val('').removeClass('alert');
          $(this).siblings('.error').remove();
          
          check_login_setup();
        });
        
        // verify password
        $('.verifyPwd').keyup(function () {
          check_login_setup();
        });

        $('.verifyPwd').blur(function () {
          check_login_setup();
        });

        $("input[name='answer1']").keyup(function () {
          check_login_setup();
        });

        $("input[name='answer1']").blur(function () {
          check_login_setup();
        });
        
        $("input[name='answer2']").keyup(function () {
          check_login_setup();
        });
        
        $("select[name='question1']").change(function () {
          check_login_setup();
        });
        
        $("select[name='question2']").change(function () {
          check_login_setup();
        });

      } // end password settings page
    
      /*******************************************************************************************
       *
       *	file input widget
       *
       ******************************************************************************************/
      if ($('.fileInputWidget').length) {
        $('.fileInputWidget').find(':file').change( function () {
        
          var fileInput = $.trim($(this).val()).replace(/^.*\\/, '');
          $(this).parent().find('.fakeInputField').val(fileInput);
          if (fileInput != "") {
            $(this).parents('form').find(':submit').prop('disabled', false);
            $(this).parents('form').find(':button').prop('disabled', false);
          } else {
            $(this).parents('form').find(':submit').prop('disabled', true);
            $(this).parents('form').find(':button').prop('disabled', true);
          }
        }); 
      } // end file input widget
    
      
    
      /*******************************************************************************************
       *
       *	access schedule page
       *
       ******************************************************************************************/
      if ($('.accessSchedule').length) {
      
        var selectDays     = $('.selectDays'),
            allDays        = selectDays.find('li'),
            weekDays       = selectDays.find('.weekday'),
            weekEnd        = selectDays.find('.weekend'),
            selectTimes    = $('.timeSelection'),
            scheduledTimes = $('#scheduledTimes'),
            timesPerDay    = $('.timesPerDay');
        
        
        // show/hide the access schedule settings
        $("#asOn").on('change', function () {
          $('#scheduleWrap').slideDown();
        });       
        $("#asOff").on('change', function () {
          $('#scheduleWrap').slideUp();
        });
        
        
        // set initial date select state
        $('#scheduledTimes').prop('disabled', true);
        // hide all days. make time selection invisible so when we show 
        // the day the time selection remain hidden
        weekDays.hide().find('.timeSelection').css('visibility', 'hidden');
        weekEnd.hide().find('.timeSelection').css('visibility', 'hidden');
        
        
        // days selections
        $('#scheduledDays').on('change', function () {
          var selectIndex = $(this).val();
          
          switch (selectIndex) {
          
          case 'everyday':
            // reset and disable days select
            scheduledTimes.prop('selectedIndex', 0);
            scheduledTimes.prop('disabled', true);
            // reset all elements and hide the days checkboxes
            selectDays.find(':checkbox').prop('checked', false);
            selectDays.find('.allDay').prop("checked", true);
            selectDays.find(':text').val("").prop('disabled', true);
            weekDays.hide();
            weekEnd.hide();
            selectDays.find('.alldays').show();
            break;
          
          case 'selectdays':
            scheduledTimes.prop('disabled', false);
            // reset the same time setting
            selectDays.find('.alldays').find(':text').val("").prop('disabled', true);
            selectDays.find('.alldays').find('.allDay').prop("checked", true);
            allDays.show();
            if (scheduledTimes.val() === 'selectTimes') {
              selectDays.find('.alldays').hide();
            }
            break;
            
          case 'weekdays':
            // reset weekend days
            weekEnd.find(':checkbox').prop('checked', false);
            weekEnd.find(':text').val("").prop('disabled', true);
            weekEnd.find('.allDay').prop('checked', true);            
            weekEnd.find('.timeSelection').css('visibility', 'hidden'); 
            weekEnd.hide();
            // reset the same time setting
            selectDays.find('.alldays').find(':text').val("").prop('disabled', true);
            selectDays.find('.alldays').find('.allDay').prop("checked", true);
            // enable the days select
            scheduledTimes.prop('disabled', false);
            weekDays.show();
            weekEnd.hide()
            if (scheduledTimes.val() === 'selectTimes') {
              selectDays.find('.alldays').hide();
            }
            
            break;
            
          case 'weekend':
            // reset week days
            weekDays.find(':checkbox').prop('checked', false);
            weekDays.find(':text').val("").prop('disabled', true);
            weekDays.find('.allDay').prop('checked', true);            
            weekDays.find('.timeSelection').css('visibility', 'hidden');
            weekDays.hide();
            // reset the same time setting
            selectDays.find('.alldays').find(':text').val("").prop('disabled', true);
            selectDays.find('.alldays').find('.allDay').prop("checked", true);        
            // enable the days select
            scheduledTimes.prop('disabled', false);
            weekEnd.show();
            weekDays.hide()
            if (scheduledTimes.val() === 'selectTimes') {
              selectDays.find('.alldays').hide();
            }            
            break;
          }
        });
        
        // enable the times select for selected days
        // this will be only active if select times for selected days is active
        selectDays.find(':checkbox').each( function (i) {
          //In iPad Safari touchstart events from checkboxes, the checkbox state has 
          // not yet been updated when the event fires. Ergo we use click here
          
          $(this).on('change', function () {
            // check the days selection
            if (scheduledTimes.val() === 'selectTimes') {
              // show the associated times seletion
              if (this.checked) {
                $(this).parents('li').find('.timeSelection').css('visibility', 'visible'); 
              } else {
                // we deselect a checkbox, reset the associated times selection
                $(this).parents('li').find(':text').val("").prop('disabled', true);
                $(this).parents('li').find('.allDay').prop("checked", true);
                $(this).parents('li').find('.timeSelection').css('visibility', 'hidden'); 
              }
            }  
          });
        });  
        
        // update selected times select if days selection changes
        scheduledTimes.on('change', function () {
          if (this.value === "selectTimes") {
            // check if we have any days already selected and enable the associated times selection
            selectDays.find(':checkbox').each( function (i) {
              if (this.checked) {
                $(this).parents('li').find('.timeSelection').css('visibility', 'visible');                 
              } else {
                // we deselect a checkbox, reset the associated times selection
                $(this).parents('li').find(':text').val("").prop('disabled', true);
                $(this).parents('li').find('.allDay').prop("checked", true);
                $(this).parents('li').find('.timeSelection').css('visibility', 'hidden'); 
              }
            });
            selectDays.find('.alldays').hide();
          }
          if (this.value === "sameTime") {
            // reset all time input and present "same time" times input
            selectDays.find(':text').val("").prop('disabled', true);
            selectDays.find('.allDay').prop("checked", true);
            selectDays.find('.timeSelection').css('visibility', 'hidden'); 
            selectDays.find('.alldays').show().find('.timeSelection').css('visibility', 'visible');
          }
        });                
        
        
        // select time for selected days input pairs
        // click 24 Hours resets the times selection inputs
        $('.selectTimesForDay').on('change', function () {
          $(this).parents('li').find(':text').prop('disabled', false);
        });
        $('.allDay').on('change', function () {
          $(this).parents('li').find(':text').val("").prop('disabled', true);
        });
        
      } // end page accessSchedule
    
    
    
      /*******************************************************************************************
       *
       *	guest network page
       *
       ******************************************************************************************/
      if ($('.guestNetwork').length) {
      
        $('.networkContainer').each(function () {
          $(this).find('.modalTable').each(function () {
            var $fakeModal = $("<span class='fakeModal'>"+$(this).text()+"</span>");
            $(this).after($fakeModal);
          });
        });
          
        // enable.disable guest networks
        $('.guestNetworks').on('change', '.enableNetwork', function () {
          if (this.checked) {
            $(this).parent().siblings().find('input, select').prop('disabled', false);
            $(this).parent().siblings().find('.modalTable').css('display', 'block');
            $(this).parent().siblings().find('.fakeModal').css('display', 'none');
          } else {
            $(this).parent().siblings().find('input, select').prop('disabled', true);
            $(this).parent().siblings().find('.modalTable').css('display', 'none');
            $(this).parent().siblings().find('.fakeModal').css('display', 'block');
          }
        });

        $('.securityOptions').on('change', function () {
          var securityOption = $(this).val(),
            thisContainer = $(this).parents('.networkContainer');
          
          switch (securityOption) {
          case 'None':  // none selected
            thisContainer.find('.pwdInput').slideUp();
            break;

          case 'WPA2-PSK':  // WPA2-PSK [AES] selected
            thisContainer.find('.pwdInput').slideDown();
            break;

          case 'WPA-AUTO-PSK':  // WPA-PSK [TKIP] + WPA2-PSK [AES] selected
            thisContainer.find('.pwdInput').slideDown();
            alert("<%getstr("alert_autopsk");%>");
            break;
          }
        });

        $('.showPwd').on('change', function () {
          var passwordInput = $(this).parents('.pwdInput').find('.pwd');
          if ($(this).is(':checked')) {
            // we change the type of the password field to text so we can see the password
            if (oldIE)
              inputPasswordTextSwitch(passwordInput,1,0);
            else
              passwordInput.attr('type', 'text');
          } else {
            if (oldIE)
              inputPasswordTextSwitch(passwordInput,0,0);
            else
              passwordInput.attr('type', 'password');
          }
        });

        // add the password strength indicator to each password field instance
        $('body').on(".pwd", "focus", function () {
            var networkName = '#' + $(this).parents('.networkContainer').find('.networkName').attr('id');

            $(this).passStrength({
              shortPass: "top_shortPass",
              longPass: "top_longPass",
              badPass: "top_badPass",
              goodPass: "top_goodPass",
              strongPass: "top_strongPass",
              baseStyle: "top_testresult",
              userid: networkName,
              messageloc: 1,
              minLength: MIN_PWD_CHARACTERS,
              maxLength: MAX_PWD_CHARACTERS
            });
        });

        if ($('.modalTable').length) {

          var $modal = $('.modalTable'),
            $screen = "<div id='modalOverlay'><div class='loadingModal'></div></div>";
            // load the overlay 
          if(!$('#modalOverlay').length)
            $('body').append($screen);

          // attach click handler on every modal link on this page
          $modal.each(function (i) {

            // we use the link index to create a unique id for each modal box
            var $thisLink = $(this),
              ajaxRet = false;

            /**
             * this is the first click only, we build the modal box and load the content
             * any further click will be dealt with a different click handler
             */
            $thisLink.on('click', function (e) {

              // build the modal box
              var $modalBox = $("<div id='modalBox' class='modalBox'></div>");

              // activate the overlay
              if($('#modalOverlay').hasClass('active'))
                $('#modalOverlay').fadeIn();
              else
                $('#modalOverlay').addClass('active');

              $('#modalOverlay').find('.loadingModal').fadeIn();

              if(typeof($thisLink.data('bssid-idx')) != "undefined" 
                && $thisLink.data('bssid-idx') !== null) {
                var urlStr = "ajax/set_bssid_idx";
                var postStr;

                if ($thisLink.parents('tr').hasClass('2g'))
                  postStr = "band=2g";
                else
                  postStr = "band=5g";

                postStr = postStr + "&bssid_idx=" + $thisLink.data('bssid-idx');

                $.ajax({
                   type: "POST",
                   async: false,
                   url: urlStr,
                   data: postStr,
                   dataType: "html",
                   success: function(data){
                        if (data == "success")
                          ajaxRet = true;
                   },
                   error: function (responseText, textStatus, XMLHttpRequest){
                        if (window.console && console.log) {
                            console.log("responseText:", responseText);
                            console.log("textStatus:", textStatus);
                            console.log("XMLHttpRequest:", XMLHttpRequest);
                        }
                   }
                });
              }

              if (ajaxRet) {
                if(!$('#modalBox').length)
                  $('body').append($modalBox);
                
                // check if we are overwriting the width with a data attribute
                // if width exsists we overwrite the css
                if(typeof($thisLink.data('popup-type')) != "undefined" && $thisLink.data('popup-type') !== null) {
                  
                  $('#modalBox').addClass($thisLink.data('popup-type'));
                  
                }

                // get the url from the link
                var modalBoxContentURL = $thisLink.attr('href'),
                  $thisModalBox = $('#modalBox');

                // and load the content into the modal box
                $thisModalBox.load(modalBoxContentURL, function () {
                  var cf = document.connectedDevicesForm;
                  var g_accessLimit = cf.accessLimit.value;
                  var acl_mac_num = cf.aclMacNum.value;
                  var g_accessRules = cf.accessRules.value;

                  function setACLSwitch()
                  {
                    if (g_accessLimit == "1") {
                      $('#aclSwitch').prop('checked', true);
                      $('.wifiDevices').find('thead').find('tr th:last').css('display', 'table-cell');
                      $('.wifiDevices').find('tbody').find('tr').find('td:last').css('display', 'table-cell');
                    } else {
                      $('#aclSwitch').prop('checked', false);
                      $('.wifiDevices').find('thead').find('tr th:last').css('display', 'none');
                      $('.wifiDevices').find('tbody').find('tr').find('td:last').css('display', 'none');
                    }
                  }

                  setACLSwitch();
                  $('.wifiDevices').tableResize(10);

                  /**
                   *  init data of each rule
                   *  status-->value  not changed-->false  changed-->true
                   *  action-->value  
                   *    checked -> unchecked -->0 (remove from blacklist)  
                   *    unchecked -> checked -->1 (add to blacklist)
                   */
                  $('.wifiDevices').find('tbody tr').each(function () {
                    $(this).data('status', false);
                    $(this).data('action', 0);
                    $(this).data('mac', $.trim($(this).find('td:eq(2)').text()));
                  });

                  // ACL on/off
                  $('#aclSwitch').on('change', function () {
                    if (g_accessLimit == "1")
                      g_accessLimit = "0";
                    else
                      g_accessLimit = "1";

                    setACLSwitch();
                  });

                  $('.wifiDevices').on('change', ':checkbox', function () {
                    var thisTr = $(this).parents('tr');

                    if ($(this).is(':checked')) {
                      if (acl_mac_num >= <% nvram_get("wlan_acl_num") %>){
                        alert("<%getstr("acl_length_64");%>");
                        $(this).prop("checked", false);
                        return false;
                      }
                      thisTr.data('action', 1);
                      acl_mac_num++;
                    } else {
                      thisTr.data('action', 0);
                      acl_mac_num--;
                    }

                    if (typeof(thisTr.data('status')) == "undefined"
                      || thisTr.data('status') === false)
                      thisTr.data('status', true);
                    else
                      thisTr.data('status', false);
                  });

                  $("#connectedDevicesForm").on('submit', function() {
                    $('.wifiDevices').find('tbody tr').each(function () {
                      if (typeof($(this).data('status')) == "undefined"
                        || $(this).data('status') === true)
                        g_accessRules = g_accessRules + $(this).data('action') + ","
                                                      + $(this).data('mac') + ";";
                    });
                    cf.accessLimit.value = g_accessLimit;
                    cf.accessRules.value = g_accessRules;
                    return true;
                  });

                  $('#connectedDevicesForm').on('click', '.refreshBtn', function (){
                    location.href='guestNetwork.html';
                  });

                  if (cf.show_wps_alert.value == "1")
                    alert("<%getstr("wps_in_process");%>");

                  // attach a close handler
                  $thisModalBox.find('.close').click(function () {
                    $thisModalBox.fadeOut();
                    $('#modalOverlay').fadeOut();
                  });
                  
                  $('#modalOverlay').find('.loadingModal').fadeOut();
                  $thisModalBox.fadeIn();
                }); // end of load content
              } else {
                if (window.console && console.log)
                  console.log("fail");
                $('#modalOverlay').fadeOut();
              }

              return false;

            }); // end first click   
          }); // end each link
        }

      } // end guestnetwork page
      
      
      /*******************************************************************************************
       *
       *	fast lane page
       *
       ******************************************************************************************/
      if ($('.fastLane').length) {
        
        if (isPhone) {
          $('.fastLaneOptions').find('.secondary').hide();
        }
        
        $('#flBasic').on('change', function () {
          if(this.checked) {
            $(this).parents('ul').find('#flBasic').siblings('.fastLaneIllustration').show();
            $(this).parents('ul').find('.secondary').hide();
          }
        });
        
        $('#fl').on('change', function () {
          if(this.checked) {
            $(this).parents('ul').find('#flBasic').siblings('.fastLaneIllustration').hide();
            $(this).parents('ul').find('.secondary').show();
          }
        
        });
        
        
        
      }
    
      
      /*******************************************************************************************
       *
       *  WPS settings page
       *
       ******************************************************************************************/
      if ($('.wpsWizard').length) {

        $("#pbc").on('change', function () {
          $('.pinSection').hide();
          $('.pbcSection').show();
        });
        
        $("#pin").on('change', function () {
          $('.pbcSection').hide();
          $('.pinSection').show();
        });
      }

      //resize the fileInputWidget
      if ($('.fileInputWidget').length 
          && $('.fileInput').length
          && $('.fakeInputField').length) {
        var restWidth = 0, realWidth = 0;
        
        realWidth = $(this).find('.fileInput').textScrollWidth();
        restWidth = $('.fileInput').width();

        if (restWidth < realWidth) {
          var adjustWidth = realWidth - restWidth;
          var fileInputWidth = $(this).find('.fileInput').innerWidth();
          var fakeInputWidth = $(this).find('.fakeInputField').innerWidth();
          if (fakeInputWidth - adjustWidth > 0) {
            $(this).find('.fileInput').css('width', fileInputWidth + adjustWidth + "px");
            $(this).find('.fakeInputField').css('width', fakeInputWidth - adjustWidth + "px");
          }
        }
      }

      $('.processButtons').find('.btn').each(function() {
        var containerLength = $(this).width();
        var textLength = $(this).textScrollWidth();
        if(textLength > containerLength) {
          var thisFontSize = $(this).css('font-size');
          var textFontSize = parseInt(thisFontSize);
          var unit = "px";

          if (textFontSize >= 15) {
            textFontSize -= 6;
            $(this).css('font-size', textFontSize + unit);
            $(this).css('width', 'auto');
            $(this).css('padding', '0px 5px 0px 5px');

            if($(this).siblings().hasClass('btn')) {
              $(this).siblings().css('font-size', textFontSize + unit);
              $(this).siblings().css('width', 'auto');
              $(this).siblings().css('padding', '0px 5px 0px 5px');
            }
          }
        }
      });

      function displayLinkStatus(design)
      {
        /* 
         * 0: disabled 
         * 1: green 
         * 2: amber 
         * 3: red 
         * 4: not connected 
        */
        var rootap_link_led_status = "<%ia_cgi_get_param("link_led","rootap");%>";
        var client_link_led_status = "<%ia_cgi_get_param("link_led","client");%>";
        var rootap_arrow_led_status = "<%ia_cgi_get_param("arrow_led","rootap");%>";
        var client_arrow_led_status = "<%ia_cgi_get_param("arrow_led","client");%>";
        var rootap_link_status_2g = "<%ia_cgi_get_param("link_status","rootap","2g");%>";
        var rootap_link_status_5g = "<%ia_cgi_get_param("link_status","rootap","5g");%>";
        var client_link_status_2g = "<%ia_cgi_get_param("link_status","client","2g");%>";
        var client_link_status_5g = "<%ia_cgi_get_param("link_status","client","5g");%>";
        var access_point_mode = "<%ia_cgi_get_param("ap_mode");%>";
        var client_2g_on = 1;
        var client_5g_on = 1;
        var rootap_2g_on = 1;
        var rootap_5g_on = 1;
        
        if (rootap_link_status_2g == 0)
          rootap_2g_on = 0; // interface with 2g root ap is off
          
        if (rootap_link_status_5g == 0)
          rootap_5g_on = 0; // interface with 5g root ap is off
        
        if (client_link_status_2g == 0)
          client_2g_on = 0; //fastlane mode or 2g ap radio is off
        else
          $("#networkStatus_2g").css('display', 'block');
          
        if (client_link_status_5g == 0)
          client_5g_on = 0; //fastlane mode or 5g ap radio is off
        else
          $("#networkStatus_5g").css('display', 'block');

        if (design == "GECKO") {
          if (access_point_mode != 1)
          {
            if (rootap_link_led_status > client_link_led_status)
            {
              rootap_link_status_2g = rootap_link_led_status;
              rootap_link_status_5g = rootap_link_led_status;
            }
            else
            {
              rootap_link_status_2g = client_link_led_status;
              rootap_link_status_5g = client_link_led_status;
            }
            
            client_link_status_2g = rootap_link_status_2g;
            client_link_status_5g = rootap_link_status_5g;

            if (rootap_link_led_status == 1 && client_link_led_status == 1)
            {
              $("#cc_blackshade").addClass('smallMarkImage');
              $("#cc_blackshade").addClass('blackShade');
              
              $("#cc_icon1").addClass('smallMarkImage');
              $("#cc_icon1").addClass('whiteCircleTop');
              $("#cc_icon2").addClass('smallMarkImage');
              $("#cc_icon2").addClass('green_routerSideImage');
              
              $("#cc_text1").addClass('letters_green_topLine');
              $("#cc_text1").html("<%getstr("green_info");%>");
              
              $("#cc_icon3").addClass('smallMarkImage');
              $("#cc_icon3").addClass('whiteCircleBottom');
              $("#cc_icon4").addClass('smallMarkImage');
              $("#cc_icon4").addClass('green_deviceSideImage');

              $("#cc_text2").addClass('letters_green_bottomLine');
              $("#cc_text2").html("<%getstr("green_info");%>");
            } else if (rootap_arrow_led_status != 0) {
              //case: rootap(amber/red)<---device(left arrow blink)
              $("#cc_blackshade").addClass('smallMarkImage');
              $("#cc_blackshade").addClass('blackShade');
              
              $("#cc_icon1").addClass('smallMarkImage');
              $("#cc_icon1").addClass('whiteEllipse');
              $("#cc_icon2").addClass('smallMarkImage');
              $("#cc_icon3").addClass('smallMarkImage');
              $("#cc_icon3").addClass('arrowLeft');
              $("#cc_text1").addClass('letters_topLine');

              if (rootap_link_led_status == 2) {
                $("#cc_icon2").addClass('amber_routerSideImage');
                $("#cc_text1").html("<%getstr("amber_info");%>");
              } else if (rootap_link_led_status == 3) {
                $("#cc_icon2").addClass('red_routerSideImage');
                $("#cc_text1").html("<%getstr("red_info");%>");
              }

              $("#cc_text2").addClass('letters_bottomLine');
              $("#cc_text2").html("<%getstr("closer_router");%>");
            } else if (client_arrow_led_status != 0) {
              //case: device(right arrow blink)--->client(amber/red)
              $("#cc_blackshade").addClass('smallMarkImage');
              $("#cc_blackshade").addClass('blackShade');

              $("#cc_icon1").addClass('smallMarkImage');
              $("#cc_icon1").addClass('whiteEllipse');
              $("#cc_icon2").addClass('smallMarkImage');
              $("#cc_icon2").addClass('arrowRight');
              $("#cc_icon3").addClass('smallMarkImage');
              $("#cc_text1").addClass('letters_topLine');

              if (client_link_led_status == 2) {
                $("#cc_icon3").addClass('amber_deviceSideImage');
                $("#cc_text1").html("<%getstr("amber_info");%>");
              } else if (client_link_led_status == 3) {
                $("#cc_icon3").addClass('red_deviceSideImage');
                $("#cc_text1").html("<%getstr("red_info");%>");
              }

              $("#cc_text2").addClass('letters_bottomLine');
              $("#cc_text2").html("<%getstr("closer_extender");%>");
            }
          }
        } else if (design == "HERO") {
          if (access_point_mode != 1)
          {
            if (rootap_link_status_2g == 0 || rootap_link_status_5g == 0)
            {
              if (rootap_link_status_2g > 0)
                rootap_link_status_5g = rootap_link_status_2g;
              else
                rootap_link_status_2g = rootap_link_status_5g;
            }
            else
            {
              if (rootap_link_status_2g > rootap_link_status_5g)
                rootap_link_status_2g = rootap_link_status_5g;
              else
                rootap_link_status_5g = rootap_link_status_2g;
            }
            
            client_link_status_2g = rootap_link_status_2g;
            client_link_status_5g = rootap_link_status_5g;

            if (rootap_link_status_2g == 1)
            {
              $("#cc_blackshade").addClass('smallMarkImage');
              $("#cc_blackshade").addClass('blackShade');
              
              $("#cc_icon1").addClass('smallMarkImage');
              $("#cc_icon1").addClass('whiteCircleTop');
              $("#cc_icon2").addClass('smallMarkImage');
              if (rootap_2g_on == 1)
                $("#cc_icon2").addClass('linkRate24g_top');
              else
                $("#cc_icon2").addClass('linkRate5g_right'); //to be improved
              
              $("#cc_text1").addClass('letters_green_topLine');
              $("#cc_text1").html("<%getstr("green_info");%>");
              
              if (rootap_2g_on == 1 && rootap_5g_on == 1)
              {
                $("#cc_icon3").addClass('smallMarkImage');
                $("#cc_icon3").addClass('whiteCircleBottom');
                $("#cc_icon4").addClass('smallMarkImage');
                $("#cc_icon4").addClass('linkRate5g_bottom');
                $("#cc_text2").addClass('letters_green_bottomLine');
                $("#cc_text2").html("<%getstr("green_info");%>");
              }
            }
            else if (rootap_link_status_2g == 2 || rootap_link_status_2g == 3)
            {
              $("#cc_blackshade").addClass('smallMarkImage');
              $("#cc_blackshade").addClass('blackShade');
              
              if (rootap_2g_on == 1 && rootap_5g_on == 1)
              {
                $("#cc_icon1").addClass('smallMarkImage');
                $("#cc_icon1").addClass('whiteEllipse');
                $("#cc_icon2").addClass('smallMarkImage');
                $("#cc_icon2").addClass('linkRate24g_left');
                $("#cc_icon3").addClass('smallMarkImage');
                $("#cc_icon3").addClass('linkRate5g_right');
                $("#cc_text1").addClass('letters_topLine');
                if (rootap_link_status_2g == 2)
                  $("#cc_text1").html("<%getstr("amber_info");%>");
                else //3
                  $("#cc_text1").html("<%getstr("red_info");%>");
              }
              else
              {
                $("#cc_icon1").addClass('smallMarkImage');
                $("#cc_icon1").addClass('whiteCircleTop');
                $("#cc_icon2").addClass('smallMarkImage');
                if (rootap_2g_on == 1)
                  $("#cc_icon2").addClass('linkRate24g_top');
                else
                  $("#cc_icon2").addClass('linkRate5g_right'); //to be improved
                
                $("#cc_text1").addClass('letters_green_topLine');
                if (rootap_link_status_2g == 2)
                  $("#cc_text1").html("<%getstr("amber_info");%>");
                else //3
                  $("#cc_text1").html("<%getstr("red_info");%>");
              }
              
              $("#cc_text2").addClass('letters_bottomLine');
              $("#cc_text2").html("<%getstr("closer_extender");%>");
            }
          }
        }

        if (access_point_mode == 1)
        {
          client_link_status_2g = rootap_link_status_2g;
          client_link_status_5g = rootap_link_status_5g;
        }

        if (access_point_mode != 1) //repeater mode
        {
          if (rootap_link_status_2g == 1 || rootap_link_status_5g == 1 
              || rootap_link_status_2g == 2 || rootap_link_status_5g == 2 
              || rootap_link_status_2g == 3 || rootap_link_status_5g == 3)
          {
            $("#title_string").html("<%getstr("extender_success_conn");%>");
          }
          else 
          {
            $("#title_string").append("<%getstr("info_fail_conn");%>");
          }
        }
        else //access point
        {
          if (client_link_status_2g == 1 || client_link_status_5g == 1
              || client_link_status_2g == 2 || client_link_status_5g == 2
              || client_link_status_2g == 3 || client_link_status_5g == 3)
          {
            $("#title_string").html("<%getstr("success_create_ap");%>");
          }
          else
          {
            $("#title_string").html("<%getstr("info_fail_conn");%>");
          }
        }

        // sta band 2.4G link status
        $("#cc_location1").addClass('confirmation');
        $("#cc_location1").addClass('location12');
        if (rootap_link_status_2g == 1)
          $("#cc_location1").addClass('best1');
        else if (rootap_link_status_2g == 2)
          $("#cc_location1").addClass('ok1');
        else if (rootap_link_status_2g == 3)
          $("#cc_location1").addClass('poor1');
        else if (rootap_link_status_2g == 0)
          $("#cc_location1").addClass('noConnected1');
        else //4
          $("#cc_location1").addClass('disable1');
          
        // sta band 5G link status
        $("#cc_location4").addClass('confirmation');
        $("#cc_location4").addClass('location34');
        if (rootap_link_status_5g == 1)
          $("#cc_location4").addClass('best4');
        else if (rootap_link_status_5g == 2)
          $("#cc_location4").addClass('ok4');
        else if (rootap_link_status_5g == 3)
          $("#cc_location4").addClass('poor4');
        else if (rootap_link_status_5g == 0)
          $("#cc_location4").addClass('noConnected4');
        else //4
          $("#cc_location4").addClass('disable4');

        // ap band 2.4G link status
        $("#cc_location2").addClass('confirmation');
        $("#cc_location2").addClass('location12');
        if (client_link_status_2g == 1)
          $("#cc_location2").addClass('best2');
        else if (client_link_status_2g == 2)
          $("#cc_location2").addClass('ok2');
        else if (client_link_status_2g == 3)
          $("#cc_location2").addClass('poor2');
        else if (client_link_status_2g == 0)
          $("#cc_location2").addClass('noConnected2');
        else //4
          $("#cc_location2").addClass('disable2');
        
        // ap band 2.4G link status
        $("#cc_location3").addClass('confirmation');
        $("#cc_location3").addClass('location34');
        if (client_link_status_5g == 1)
          $("#cc_location3").addClass('best3');
        else if (client_link_status_5g == 2)
          $("#cc_location3").addClass('ok3');
        else if (client_link_status_5g == 3)
          $("#cc_location3").addClass('poor3');
        else if (client_link_status_5g == 0)
          $("#cc_location3").addClass('noConnected3');
        else //4
          $("#cc_location3").addClass('disable3');

      }
      
      function set_design(design)
      {
        if ($('.success').length)
          displayLinkStatus(design);

        if (design == "GECKO") {
          if ($('.connectedDevices').length) {
            if ("<% nvram_get("wire_mode") %>" == "1")
              $('#wired_devices').css('display', 'none');
          }
        }
      }

      set_design("<%basic_cgi_get_param("product_design")%>");

      if ($('.tipLink').length) {
        $('body').tipload();
        $('.tipLink').click(function () {
          $('body').msgshow($(this).data('msg'));
        });
      }

      // iOS hack: add optgroup into select to avoid option content truncation
      if (navigator.userAgent.match(/(iPad|iPhone|iPod)/i)) {
        var selects = document.querySelectorAll("select");
        for (var i = 0; i < selects.length; i++) {
          selects[i].appendChild(document.createElement("optgroup"));
        }
      }
    }); // end ready function

  /*******************************************************************************************
   *
   *  tool tip function
   *
   *  usage: obj.tip();
   *     eg: $('.availableNetworks').find('tr').find('td:eq(0)').find('span:eq(1)').tip();
   *
   *  note: tip content come from the innerText of obj
   ******************************************************************************************/
    $.fn.tip = function () {
      this.each(function (i) {
        var showTip = true;
        // judge if the text is overflow in box container like td of table
        if($(this).parents('table').length) {
          var restWidth = 0, realWidth = 0, siblingsWidth = 0;
          // count the length of siblings
          $(this).siblings().each(function () {
            // do not count the element if it is hidden
            if ($(this).css('display') !== 'none') {
              var margin = $(this).outerWidth(true) - $(this).outerWidth(),
                  padding = $(this).innerWidth() - $(this).width();
              siblingsWidth += $(this).width() + margin + padding;
            }
          });

          restWidth = $(this).parent().width() - siblingsWidth;
          realWidth = $(this).textScrollWidth();
          if(realWidth <= restWidth)
            showTip = false;
        }

        if(showTip) {
          var tip = $(this).text(),
            tipIndex = i + 1;

          $(this).addClass('tooltip-container');

          $(this).on('mouseover', function (event) {
            var tipBoxID='tipBox' + tipIndex,
                $tipObj = $("<div class='tooltip-realbox' id='"+tipBoxID+"'><span class='tooltip'>"+tip+"</span></div>");
            
            $tipObj.prependTo($('#content'));

            var evt = event?event:(window.event?window.event:null),
                $thisTipBox = $('#' + tipBoxID),
                tipBox = { top: evt.pageY-65 + "px", left: evt.pageX-5 + "px"}; // 65:-70px(header)+5px

            $thisTipBox.css(tipBox);
            $thisTipBox.find('.tooltip').css('margin-left', -($thisTipBox.find('.tooltip').width()/2));

            $(this).on('mouseleave', function () {
              $thisTipBox.remove();
            });
          });
        }
      });
    };

  /*******************************************************************************************
   *
   *  popup message function with loading image
   *
   *  @param msg (string) - the message we show in the loadingBox
   *
   *  usage: $('body').msgload(),$('body').msgshow(msg),$('body').msgclean();
   *
   *  append below html to '<body>':
   *
   *      <div id='modalOverlay'></div>
   *      <div id='loadingBox' class='loadingBox'>
   *        <p id='loadingMsg'>msg</p>
   *      </div>
   ******************************************************************************************/
    $.fn.msgload = function () {
      var msgStatus = false,
        msgContainer = "<div id='loadingBox' class='loadingBox'><p id='loadingMsg'></p></div>";

      if(typeof($(this).data('msgStatus')) == "undefined" 
        || $(this).data('msgStatus') === false) {
        // avoid the conflict with the modal function(learn more>>)
        if(!$('#modalOverlay').length)
          $(this).append("<div id='modalOverlay'></div>");
        
        $(this).append(msgContainer);
        msgStatus = true;
        $(this).data('msgStatus', msgStatus);
      }
    };

    $.fn.msgshow = function (msg) {
      if($(this).data('msgStatus') === true) {
        $('#loadingMsg').html(msg);
        if(!$('#modalOverlay').hasClass('active'))
          $('#modalOverlay').addClass('active');
        $('#modalOverlay').fadeIn();
        $('#loadingBox').fadeIn();
      }
    };

    $.fn.msgclean = function () {
      if($(this).data('msgStatus') === true) {
        $('#loadingBox').fadeOut();
        $('#modalOverlay').fadeOut();
      }
    };
    
    $.fn.tipload = function () {
      var msgStatus = false,
      msgContainer = "<div id='loadingBox' class='modalBox hidden'><p id='loadingMsg' class='recommendation'></p>  <a class='btn primary close'><%getstr("ok_lower");%></a></div>";

      if(typeof($(this).data('msgStatus')) == "undefined" 
        || $(this).data('msgStatus') === false) {
        // avoid the conflict with the modal function(learn more>>)
        if(!$('#modalOverlay').length)
          $(this).append("<div id='modalOverlay'></div>");

        $(this).append(msgContainer);
        msgStatus = true;
        $(this).data('msgStatus', msgStatus);
      }

      $('#loadingBox').find('.close').click(function () {
        $('body').msgclean();
      });
    };

  /*******************************************************************************************
   *
   *  show srcoll-bar if rows of table more than fixed number
   *
   *  @param rowNum (int) - max length of visiable rows 
   *
   *  usage: $('.wifiDevices').tableResize(10);
   *
   ******************************************************************************************/
    $.fn.tableResize = function (rowNum) {
      if (!$(this).is('table')) {
        if (window.console && console.log)
          console.log("this is not a table");
        return false;
      }

      if (isNaN(rowNum)) {
        if (window.console && console.log)
          console.log("rowNum is not a number!");
        return false;
      }

      if (rowNum <=0 ) {
        if (window.console && console.log)
          console.log("rowNum is:", rowNum);
        return false;
      }

      var parentDisplay = true,     // each element height is 0 if it's container is hidden
          trNum = 0, 
          trHeight = 0, 
          scrollPaneHeight;

      if ($('.wifiSettings').length) {
        /* make sure this table has attr display:block */
        if ($(this).parents('.sectionWrap').length && !$(this).parents('.sectionWrap').hasClass('open'))
          parentDisplay = false;
      }

      if (!parentDisplay) {
        /* action temporary show the table */
        if ($('.wifiSettings').length) {
          $(this).parents('.sectionWrap')
                 .addClass('open')
                 .find('.sectionDetails')
                 .slideDown("fast");
        }
      }

      $(this).find('tbody').find('tr').each(function() {
        trNum++;
        if (trNum <= rowNum)
          trHeight = trHeight + $(this).height() + $(this).outerHeight(true) - $(this).outerHeight();
      });

      if (trNum > rowNum) {
        if($(this).find('thead').css('display') != 'none')
          scrollPaneHeight = $(this).find('thead').height() + trHeight;
        else
          scrollPaneHeight = trHeight;

        if(!$(this).parents('.scrollPane').length)
          $(this).wrap("<div class='scrollPane'></div>");     /* need a div to set attr overflow-y:scroll */
        $(this).parents('.scrollPane').css('height', scrollPaneHeight);
        $(this).parents('.scrollPane').css('overflow-y', 'scroll');
      } else {
        if($(this).parents('.scrollPane').length)
          $(this).unwrap();
      }

      if (!parentDisplay) {
        /* make table return to default state */
        if ($('.wifiSettings').length) {
          $(this).parents('.sectionWrap')
                 .removeClass('open')
                 .find('.sectionDetails')
                 .slideUp("fast");
        }
      }

    };

    /**
     *  get text srcollwidth if it overflow the container
     */
    $.fn.textScrollWidth = function () {
      var scrollWidth = 0;
      $("<span id='scrollWidth' style='display:inline-block;'></span>").prependTo($('body'));
      $('#scrollWidth').css('font-size', $(this).css('font-size')).text($(this).text());
      scrollWidth = $('#scrollWidth').width();
      $('#scrollWidth').remove();
      if (window.console && console.log)
        console.log("element real width="+scrollWidth);
      return scrollWidth;
    };
    
}(jQuery));
